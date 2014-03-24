#include "map_collidable.h"

#include "accelerators.h"
#include "kinematics.h"
#include "collision_rectangle.h"
#include "map.h"

using boost::optional;

namespace {
optional<units::Game> testMapCollision(
      const Map& map,
      const Rectangle& rectangle,
      sides::SideType direction) {
   std::vector<CollisionTile> tiles(map.getCollidingTiles(rectangle));
   for (size_t i = 0; i < tiles.size(); ++i) {
      const sides::SideType side = sides::opposite_side(direction);
      const units::Game position = sides::vertical(side) ?
         rectangle.center_x() :
         rectangle.center_y();
      const optional<units::Game> maybe_position(
            tiles[i].testCollision(side, position));
      if (maybe_position) {
         return maybe_position;
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
   accelerator.updateVelocity(kinematics_x, elapsed_time_ms);
   // Calculate delta
   const units::Game delta = kinematics_x.velocity * elapsed_time_ms; 
   if (delta > 0.0f) {
      // Check collision in the direction of delta.
      const sides::SideType direction = sides::RIGHT_SIDE;
      optional<units::Game> maybe_position = testMapCollision(
            map,
            collision_rectangle.rightCollision(kinematics_x.position, kinematics_y.position, delta),
            direction);
      // React to collision
      if (maybe_position) {
         kinematics_x.position = *maybe_position - collision_rectangle.boundingBox().right();
         onCollision(direction, true);
      } else {
         kinematics_x.position += delta;
         onDelta(direction);
      }

      // Check collision in other direction.
      const sides::SideType opposite_direction = sides::opposite_side(direction);
      maybe_position = testMapCollision(
            map,
            collision_rectangle.leftCollision(kinematics_x.position, kinematics_y.position, 0),
            opposite_direction);
      if (maybe_position) {
         kinematics_x.position = *maybe_position - collision_rectangle.boundingBox().left();
         onCollision(opposite_direction, false);
      }
   } else {
      // Check collision in the direction of delta.
      const sides::SideType direction = sides::LEFT_SIDE;
      optional<units::Game> maybe_position = testMapCollision(
            map,
            collision_rectangle.leftCollision(kinematics_x.position, kinematics_y.position, delta),
            direction);
      // React to collision
      if (maybe_position) {
         kinematics_x.position = *maybe_position - collision_rectangle.boundingBox().left();
         onCollision(direction, true);
      } else {
         kinematics_x.position += delta;
         onDelta(direction);
      }

      // Check collision in other direction.
      const sides::SideType opposite_direction = sides::opposite_side(direction);
      maybe_position = testMapCollision(
            map,
            collision_rectangle.rightCollision(kinematics_x.position, kinematics_y.position, 0),
            opposite_direction);
      if (maybe_position) {
         kinematics_x.position = *maybe_position - collision_rectangle.boundingBox().right();
         onCollision(opposite_direction, false);
      }
   }
}

void MapCollidable::updateY(
      const CollisionRectangle& collision_rectangle,
      const Accelerator& accelerator,
      const Kinematics& kinematics_x, Kinematics& kinematics_y, 
      units::MS elapsed_time_ms, const Map& map) {
   accelerator.updateVelocity(kinematics_y, elapsed_time_ms);
   // Calculate delta
   const units::Game delta = kinematics_y.velocity * elapsed_time_ms;
   if (delta > 0) {
      // Check collision in the direction of delta.
      const sides::SideType direction = sides::BOTTOM_SIDE;
      optional<units::Game> maybe_position = testMapCollision(
            map,
            collision_rectangle.bottomCollision(kinematics_x.position, kinematics_y.position, delta),
            direction);
      // React to collision
      if (maybe_position) {
         kinematics_y.position = *maybe_position - collision_rectangle.boundingBox().bottom();
         onCollision(direction, true);
      } else {
         kinematics_y.position += delta;
         onDelta(direction);
      }

      // Check collision in other direction.
      const sides::SideType opposite_direction = sides::opposite_side(direction);
      maybe_position = testMapCollision(
            map,
            collision_rectangle.topCollision(kinematics_x.position, kinematics_y.position, 0),
            opposite_direction);
      if (maybe_position) {
         kinematics_y.position = *maybe_position - collision_rectangle.boundingBox().top();
         onCollision(opposite_direction, false);
      }
   } else {
      const sides::SideType direction = sides::TOP_SIDE;
      optional<units::Game> maybe_position = testMapCollision(
            map,
            collision_rectangle.topCollision(kinematics_x.position, kinematics_y.position, delta),
            direction);
      // React to collision
      if (maybe_position) {
         kinematics_y.position = *maybe_position - collision_rectangle.boundingBox().top();
         onCollision(direction, true);
      }
      else {
         kinematics_y.position += delta;
         onDelta(direction);
      }

      // Check collision in other direction.
      const sides::SideType opposite_direction = sides::opposite_side(direction);
      maybe_position = testMapCollision(
            map,
            collision_rectangle.bottomCollision(kinematics_x.position, kinematics_y.position, 0),
            opposite_direction);
      if (maybe_position) {
         kinematics_y.position = *maybe_position - collision_rectangle.boundingBox().bottom();
         onCollision(opposite_direction, false);
      }
   }
}
