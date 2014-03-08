#ifndef MAP_COLLIDABLE_H_
#define MAP_COLLIDABLE_H_

#include "units.h"

struct Accelerator;
struct CollisionRectangle;
struct Kinematics;
struct Map;

struct MapCollidable {
   enum SideType {
      TOP_SIDE,
      BOTTOM_SIDE,
      LEFT_SIDE,
      RIGHT_SIDE,
   };

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

   virtual void onCollision(SideType side, bool is_delta_direction) = 0;
   virtual void onDelta(SideType side) = 0;

   virtual ~MapCollidable() {}
};

#endif // MAP_COLLIDABLE_H_