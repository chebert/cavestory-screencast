#include "map_collidable.h"

#include "accelerators.h"
#include "kinematics.h"
#include "collision_rectangle.h"
#include "map.h"

using boost::optional;

std::vector<Tile2D> MapCollidable::_debug_colliding_tiles;
std::vector<Tile2D> MapCollidable::_debug_opposite_colliding_tiles;

namespace {
   struct CollisionInfo {
      units::Game position;
      Tile2D tile_position;
      tiles::TileType tile_type;
   };
   optional<CollisionInfo> testMapCollision(
         const Map& map,
         const Rectangle& rectangle,
         sides::SideType direction,
         const boost::optional<tiles::TileType>& maybe_ground_tile) {
      std::vector<CollisionTile> tiles(map.getCollidingTiles(rectangle, direction));
      for (size_t i = 0; i < tiles.size(); ++i) {
         const sides::SideType side = sides::opposite_side(direction);
         const units::Game perpendicular_position = sides::vertical(side) ?
            rectangle.center_x() :
            rectangle.center_y();
         const units::Game leading_position = rectangle.side(direction);
         const bool should_test_slopes = sides::vertical(side);
         const CollisionTile::TestCollisionInfo test_info(
               tiles[i].testCollision(
                  side, perpendicular_position, leading_position, should_test_slopes));
         if (test_info.is_colliding) {
            const CollisionInfo info = { test_info.position, tiles[i].position(), tiles[i].tile_type() };
            return info;
         } else if (maybe_ground_tile && direction == sides::BOTTOM_SIDE) {
            const tiles::TileType tall_slope =
               tiles::TileType().set(tiles::SLOPE).set(tiles::TALL_SLOPE);
            if ((maybe_ground_tile->test(tiles::SLOPE) &&
                tiles[i].tile_type()[tiles::SLOPE]) ||
                (maybe_ground_tile->test(tiles::WALL) &&
                 (tall_slope & tiles[i].tile_type()) == tall_slope)) {
               const CollisionInfo info = { test_info.position, tiles[i].position(), tiles[i].tile_type() };
               return info;
            }
         }
      }
      return boost::none;
   }
}

void MapCollidable::updateX(
      const CollisionRectangle& collision_rectangle,
      const Accelerator& accelerator,
      Kinematics& kinematics_x, const Kinematics& kinematics_y, 
      units::MS elapsed_time_ms, const Map& map) {
   update(
         collision_rectangle,
         accelerator,
         kinematics_x, kinematics_y,
         elapsed_time_ms, map,
         boost::none,
         kinematics_x, X_AXIS);
}

void MapCollidable::updateY(
      const CollisionRectangle& collision_rectangle,
      const Accelerator& accelerator,
      const Kinematics& kinematics_x, Kinematics& kinematics_y, 
      units::MS elapsed_time_ms, const Map& map,
      const boost::optional<tiles::TileType>& maybe_ground_tile) {
   update(
         collision_rectangle,
         accelerator,
         kinematics_x, kinematics_y,
         elapsed_time_ms, map,
         maybe_ground_tile,
         kinematics_y, Y_AXIS);
}

void MapCollidable::update(
      const CollisionRectangle& collision_rectangle,
      const Accelerator& accelerator,
      const Kinematics& kinematics_x, const Kinematics& kinematics_y,
      units::MS elapsed_time_ms, const Map& map,
      const boost::optional<tiles::TileType>& maybe_ground_tile,
      Kinematics& kinematics, AxisType axis) {
   accelerator.updateVelocity(kinematics, elapsed_time_ms);
   // Calculate delta
   const units::Game delta = kinematics.velocity * elapsed_time_ms;
   // Check collision in the direction of delta.
   const sides::SideType direction = axis == X_AXIS ?
      (delta > 0 ? sides::RIGHT_SIDE : sides::LEFT_SIDE) :
      (delta > 0 ? sides::BOTTOM_SIDE : sides::TOP_SIDE);
   {
      optional<CollisionInfo> maybe_info = testMapCollision(
            map,
            collision_rectangle.collision(direction, kinematics_x.position, kinematics_y.position, delta),
            direction,
            maybe_ground_tile);
      // React to collision
      if (maybe_info) {
         kinematics.position = maybe_info->position - collision_rectangle.boundingBox().side(direction);
         _debug_colliding_tiles.push_back(maybe_info->tile_position);
         onCollision(direction, true, maybe_info->tile_type);
      } else {
         kinematics.position += delta;
         onDelta(direction);
      }
   }

   // Check collision in other direction.
   const sides::SideType opposite_direction = sides::opposite_side(direction);
   optional<CollisionInfo> maybe_info = testMapCollision(
         map,
         collision_rectangle.collision(opposite_direction, kinematics_x.position, kinematics_y.position, 0),
         opposite_direction,
         boost::none);
   if (maybe_info) {
      kinematics.position = maybe_info->position - collision_rectangle.boundingBox().side(opposite_direction);
      _debug_opposite_colliding_tiles.push_back(maybe_info->tile_position);
      onCollision(opposite_direction, false, maybe_info->tile_type);
   }
}
