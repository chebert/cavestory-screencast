#ifndef COLLISION_TILE_H_
#define COLLISION_TILE_H_

#include <boost/optional.hpp>

#include "side_type.h"
#include "tile_type.h"
#include "units.h"

struct CollisionTile {
   CollisionTile(units::Tile row, units::Tile col, tiles::TileType tile_type) :
      row_(row),
      col_(col),
      tile_type_(tile_type) {}

   // returns boost::none if there was no collision,
   // otherwise the position of the collision on the same axis of side
   //
   // side: the side of the tile that is being collided with
   // perpendicular_position: the position on the tile on the opposite axis of side
   // leading_position: position of the leading edge of the colliding entity
   // should_test_slopes: whether slopes should be considered for collision
   boost::optional<units::Game> testCollision(
         sides::SideType side,
         units::Game perpendicular_position,
         units::Game leading_position,
         bool should_test_slopes) const;

  private:
   units::Tile row_, col_;
   tiles::TileType tile_type_;
};

#endif // COLLISION_TILE_H_
