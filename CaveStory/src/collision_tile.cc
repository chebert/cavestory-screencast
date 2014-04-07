#include "collision_tile.h"

using namespace sides;
using namespace tiles;

namespace {
   inline float get_slope(const TileType& tile_type) {
      const TileType right_top = TileType().set(RIGHT_SLOPE).set(TOP_SLOPE);
      const TileType left_bottom = TileType().set(LEFT_SLOPE).set(BOTTOM_SLOPE);
      const bool is_positive =
         ((right_top & tile_type) == right_top) ||
         ((left_bottom & tile_type) == left_bottom);
      return is_positive ? 0.5f : -0.5f;
   }

   inline units::Game get_offset(const TileType& tile_type) {
      {
         const TileType left_top_tall =
            TileType().set(LEFT_SLOPE).set(TOP_SLOPE).set(TALL_SLOPE);
         const TileType right_bottom_short =
            TileType().set(RIGHT_SLOPE).set(BOTTOM_SLOPE).set(SHORT_SLOPE);
         if ((left_top_tall & tile_type) == left_top_tall ||
             (right_bottom_short & tile_type) == right_bottom_short) {
            return units::tileToGame(1);
         }
      }

      const TileType left_bottom_tall =
         TileType().set(LEFT_SLOPE).set(BOTTOM_SLOPE).set(TALL_SLOPE);
      const TileType right_top_short =
         TileType().set(RIGHT_SLOPE).set(TOP_SLOPE).set(SHORT_SLOPE);
      if ((left_bottom_tall & tile_type) == left_bottom_tall ||
          (right_top_short & tile_type) == right_top_short) {
         return 0.0f;
      }

      return units::kHalfTile;
   }
}

CollisionTile::TestCollisionInfo CollisionTile::testCollision(
      sides::SideType side,
      units::Game perpendicular_position,
      units::Game leading_position,
      bool should_test_slopes) const {
   TestCollisionInfo info = { false, leading_position };
   if (tile_type_[WALL]) {
      info.is_colliding = true;
      if (side == TOP_SIDE)
         info.position = units::tileToGame(row_);
      else if (side == BOTTOM_SIDE)
         info.position = units::tileToGame(row_ + 1);
      else if (side == LEFT_SIDE)
         info.position = units::tileToGame(col_);
      else
         info.position = units::tileToGame(col_ + 1);
   } else if (should_test_slopes &&
              tile_type_[SLOPE] &&
              !tile_type_[slope_flag_from_side(side)]) {
      const units::Game row = units::tileToGame(row_);
      const units::Game col = units::tileToGame(col_);
      const float slope = get_slope(tile_type_);
      const units::Game offset = get_offset(tile_type_);
      const units::Game calculated_position = sides::vertical(side) ?
         slope * (perpendicular_position - col) + offset + row :
         (perpendicular_position - row - offset) / slope + col;

      const bool is_colliding = is_max(side) ?
         leading_position <= calculated_position :
         leading_position >= calculated_position;

      info.is_colliding = is_colliding;
      info.position = calculated_position;
   }
   return info;
}
