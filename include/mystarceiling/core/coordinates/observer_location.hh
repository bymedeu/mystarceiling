#pragma once

#include <mystarceiling/core/coordinates/latitude.hh>
#include <mystarceiling/core/coordinates/longitude.hh>
namespace msc::core::coordinates
{

    class ObserverLocation
    {
    public:
        [[nodiscard]] constexpr ObserverLocation(Latitude latitude,
                                                 Longitude longitude) noexcept;


        [[nodiscard]] constexpr Latitude get_latitude() const noexcept;
        [[nodiscard]] constexpr Longitude get_longitude() const noexcept;

    private:
        Latitude latitude_;
        Longitude longitude_;
    };

    constexpr ObserverLocation::ObserverLocation(Latitude latitude,
                                                 Longitude longitude) noexcept :
        latitude_{latitude}, longitude_{longitude}
    {}

    constexpr Latitude ObserverLocation::get_latitude() const noexcept
    {
        return latitude_;
    }
    constexpr Longitude ObserverLocation::get_longitude() const noexcept
    {
        return longitude_;
    }


} // namespace msc::core::coordinates
