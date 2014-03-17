#ifndef KINEMATICS_H_
#define KINEMATICS_H_

#include "units.h"

struct Kinematics {
   Kinematics(
         units::Game position,
         units::Velocity velocity):
      position(position),
      velocity(velocity) {}

   units::Game position;
   units::Velocity velocity;

   units::Game delta(units::MS elapsed_time) { return velocity * elapsed_time; }
};

#endif // KINEMATICS_H_
