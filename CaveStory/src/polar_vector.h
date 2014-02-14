#ifndef POLAR_VECTOR_H_
#define POLAR_VECTOR_H_

#include "units.h"

struct PolarVector {
   PolarVector(units::Game magnitude, units::Degrees angle) : magnitude(magnitude), angle(angle) {}
   units::Game magnitude;
   units::Degrees angle;

   units::Game get_x() const { return magnitude * units::cos(angle); }
   units::Game get_y() const { return magnitude * units::sin(angle); }
};

#endif // POLAR_VECTOR_H_