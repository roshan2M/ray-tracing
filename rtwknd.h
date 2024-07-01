#ifndef RTWKND_H
#define RTWKND_H

#include <algorithm>
#include <cmath>
#include <random>
#include <iostream>
#include <limits>
#include <memory>

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

// Returns the radian measure equal to degrees.
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Returns a random double in [0, 1).
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

// Returns a random double in [min, max).
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

// Common Headers
#include "color.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
