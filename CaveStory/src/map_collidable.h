#ifndef MAP_COLLIDABLE_H_
#define MAP_COLLIDABLE_H_

#include "side_type.h"
#include "units.h"

struct Accelerator;
struct CollisionRectangle;
struct Kinematics;
struct Map;

struct MapCollidable {
   void updateX(
      const CollisionRectangle& collision_rectangle,
      const Accelerator& accelerator,
      Kinematics& kinematics_x, const Kinematics& kinematics_y, 
      units::MS elapsed_time_ms, const Map& map);
   void updateY(
      const CollisionRectangle& collision_rectangle,
      const Accelerator& accelerator,
      const Kinematics& kinematics_x, Kinematics& kinematics_y, 
      units::MS elapsed_time_ms, const Map& map);

   virtual void onCollision(sides::SideType side, bool is_delta_direction) = 0;
   virtual void onDelta(sides::SideType side) = 0;

   virtual ~MapCollidable() {}
  private:
   enum AxisType {
      X_AXIS,
      Y_AXIS
   };
   void update(
      const CollisionRectangle& collision_rectangle,
      const Accelerator& accelerator,
      const Kinematics& kinematics_x, const Kinematics& kinematics_y,
      units::MS elapsed_time_ms, const Map& map,
      Kinematics& kinematics, AxisType axis);
};

#endif // MAP_COLLIDABLE_H_
