////
// Created by medeu on 12/29/24.
//
#include <string.h>

struct celestial_star {
    long id;                 // Unique star ID (e.g., Gaia DR3 ID).
    char name[50];           // Name of the star (e.g., "Sirius").

    long ra;                 // Right Ascension (degrees * 1,000,000).
                             // Example: 101.2875 -> 101287500.

    long dec;                // Declination (degrees * 1,000,000).
                             // Example: -16.7161 -> -16716100.

    long magnitude;          // Apparent magnitude * 1,000.
                             // Example: -1.46 -> -1460.

    long distance;           // Distance to the star in light-years.
                             // Example: 8.6 -> 8.

    char spectral_type[5];   // Spectral type (e.g., "G2V", "A1V").

    long color_index;        // Color index (B-V) * 1,000.
                             // Example: 0.00 -> 0, 1.23 -> 1230.

    long proper_motion;      // Proper motion (arcsec/year * 1,000,000).
                             // Example: 0.132 -> 132000.

    long radial_velocity;    // Radial velocity in m/s.
                             // Example: -5.5 km/s -> -5500.

    char date[50];           // Observation date (ISO 8601 format).
                             // Example: "2016-01-01T00:00:00+00:00".
};

#ifndef DATA_COLLECTOR_H
#define DATA_COLLECTOR_H

#endif //DATA_COLLECTOR_H
