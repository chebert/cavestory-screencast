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

   struct TestCollisionInfo {
      bool is_colliding;
      units::Game position;
   };
   // info.is_colliding is true if there was a collision
   // info.position is the position where a collision occured/would occur
   //
   // side: the side of the tile that is being collided with
   // perpendicular_position: the position on the tile on the opposite axis of side
   // leading_position: position of the leading edge of the colliding entity
   // should_test_slopes: whether slopes should be considered for collision
   TestCollisionInfo testCollision(
         sides::SideType side,
         units::Game perpendicular_position,
         units::Game leading_position,
         bool should_test_slopes) const;

   tiles::TileType tile_type() const { return tile_type_; }
   Tile2D position() const { return Tile2D(col_, row_); }

  private:
   units::Tile row_, col_;
   tiles::TileType tile_type_;
};

#endif // COLLISION_TILE_H_
