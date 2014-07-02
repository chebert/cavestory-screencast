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

struct Kinematics2D {
   Position2D position() const { return Position2D(x.position, y.position); }
   Velocity2D velocity() const { return Velocity2D(x.velocity, y.velocity); }
   Kinematics x, y;
};

#endif // KINEMATICS_H_
