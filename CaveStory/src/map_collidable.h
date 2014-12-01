#ifndef MAP_COLLIDABLE_H_
#define MAP_COLLIDABLE_H_

#include <boost/optional.hpp>
#include <vector>

#include "side_type.h"
#include "tile_type.h"
#include "units.h"

struct Accelerator;
struct CollisionRectangle;
struct Kinematics;
struct Map;

struct MapCollidable {
   enum CollisionType {
      BOUNCING_COLLISION,
      STICKY_COLLISION
   };

   MapCollidable(CollisionType collision_type) : collision_type_(collision_type) {}

   void updateX(
      const CollisionRectangle& collision_rectangle,
      const Accelerator& accelerator,
      Kinematics& kinematics_x, const Kinematics& kinematics_y,
      units::MS elapsed_time_ms, const Map& map);
   void updateY(
      const CollisionRectangle& collision_rectangle,
      const Accelerator& accelerator,
      const Kinematics& kinematics_x, Kinematics& kinematics_y, 
      units::MS elapsed_time_ms, const Map& map,
      const boost::optional<tiles::TileType>& maybe_ground_tile);

   virtual void onCollision(sides::SideType side, bool is_delta_direction, const tiles::TileType& tile_type) = 0;
   virtual void onDelta(sides::SideType side) = 0;

   virtual ~MapCollidable() {}


   static std::vector<Tile2D> _debug_colliding_tiles;
   static std::vector<Tile2D> _debug_opposite_colliding_tiles;
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
      const boost::optional<tiles::TileType>& maybe_ground_tile,
      Kinematics& kinematics, AxisType axis);

   CollisionType collision_type_;
};

#endif // MAP_COLLIDABLE_H_
