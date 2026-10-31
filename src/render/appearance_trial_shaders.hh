#pragma once

namespace msc::render::trial
{
    inline constexpr const char* star_vertex = R"glsl(
#version 330 core
layout(location=0) in vec3 enu;
layout(location=1) in float magnitude;
layout(location=2) in vec3 catalogue_colour;
layout(location=3) in uvec2 key;
uniform vec2 framebuffer_size;
uniform float support_radius, brightness, limiting_magnitude;
uniform float strength, zenith_fraction, colour_delay;
uniform vec3 bands[24];
out vec2 pixel_offset;
flat out vec3 star_light;
uint mixed(uint v) {
    v=(v^(v>>16u))*0x21f0aaadu;
    v=(v^(v>>15u))*0x735a2d97u;
    return v^(v>>15u);
}
vec3 twinkle() {
    float h=1.0-clamp(enu.z,0.0,1.0);
    float factor=zenith_fraction+(1.0-zenith_fraction)*h*h;
    vec3 signal=vec3(0.0);
    for(int i=0;i<24;++i) {
        uint n=mixed(key.x^mixed(key.y)^((uint(i)+719u)*0x9e3779b9u));
        float offset=float(n>>8u)/16777216.0;
        float phase=fract(bands[i].x+offset);
        float delay=bands[i].z*colour_delay*h;
        signal+=bands[i].y*cos(6.283185307179586*
                    (vec3(phase)+vec3(-delay,0.0,delay)));
    }
    vec3 deviation=strength*factor*signal;
    // Approved baseline response; not an exposure-integrated physical model.
    return vec3(1.0)+0.9*tanh(10.0*deviation/0.9);
}
vec3 perceived_colour(vec3 light) {
    // Visual approximation: retain bright-star colour, fade faint stars to grey.
    float saturation=1.0-smoothstep(1.0,5.0,magnitude);
    float luminance=dot(light,vec3(0.2126,0.7152,0.0722));
    return mix(vec3(luminance),light,saturation);
}
void main() {
    vec2 corner=vec2((gl_VertexID&1)==0 ? -1.0:1.0,
                     (gl_VertexID&2)==0 ? -1.0:1.0);
    pixel_offset=corner*support_radius;
    vec2 centre=vec2(enu.x*framebuffer_size.y/framebuffer_size.x,enu.y);
    vec2 expanded=centre+2.0*pixel_offset/framebuffer_size*enu.z;
    gl_Position=vec4(expanded,0.0,enu.z);
    star_light=perceived_colour(catalogue_colour*twinkle())*brightness*
        pow(10.0,-0.4*clamp(magnitude,-30.0,30.0));
    if(enu.z<=0.0 || magnitude>limiting_magnitude) {
        gl_Position=vec4(2.0,2.0,2.0,1.0);
        star_light=vec3(0.0);
    }
}
)glsl";

    inline constexpr const char* star_fragment = R"glsl(
#version 330 core
in vec2 pixel_offset;
flat in vec3 star_light;
uniform float core_width, halo_width, halo_fraction, ray_fraction;
uniform float ray_width, ray_length;
uniform int ray_count;
uniform vec4 rays[24];
out vec4 colour;
const float pi=3.141592653589793;
float pixel_width(float sigma) {
    // Approximate convolution with a one-pixel box by adding its variance.
    return sqrt(sigma*sigma+1.0/12.0);
}
float gaussian(float width) {
    width=pixel_width(width);
    return exp(-0.5*dot(pixel_offset,pixel_offset)/(width*width)) /
        (2.0*pi*width*width);
}
float ray_profile(vec4 ray) {
    vec2 direction=ray.xy;
    float along=dot(pixel_offset,direction);
    float across=dot(pixel_offset,vec2(-direction.y,direction.x));
    float length=pixel_width(ray_length*ray.z);
    float width=pixel_width(ray_width);
    if(along<0.0) return 0.0;
    vec2 q=vec2(along/length,across/width);
    return ray.w*exp(-0.5*dot(q,q))/(pi*length*width);
}
void main() {
    float profile=(1.0-halo_fraction-ray_fraction)*gaussian(core_width)+
        halo_fraction*gaussian(halo_width);
    float ray_sum=0.0;
    for(int i=0;i<ray_count;++i) ray_sum+=ray_profile(rays[i]);
    colour=vec4(star_light*(profile+ray_fraction*ray_sum),0.0);
}
)glsl";

    inline constexpr const char* display_vertex = R"glsl(
#version 330 core
out vec2 uv;
void main() {
    vec2 p=vec2((gl_VertexID<<1)&2,gl_VertexID&2);
    uv=p;
    gl_Position=vec4(p*2.0-1.0,0.0,1.0);
}
)glsl";

    inline constexpr const char* display_fragment = R"glsl(
#version 330 core
in vec2 uv;
uniform sampler2D accumulated;
out vec4 colour;
vec3 srgb(vec3 light) {
    return mix(1.055*pow(light,vec3(1.0/2.4))-0.055,12.92*light,
               lessThanEqual(light,vec3(0.0031308)));
}
void main() {
    vec3 light=max(texture(accumulated,uv).rgb,vec3(0.0));
    light/=1.0+max(light.r,max(light.g,light.b));
    colour=vec4(srgb(light),1.0);
}
)glsl";
} // namespace msc::render::trial
