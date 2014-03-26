#include "collision_tile.h"

using namespace sides;

boost::optional<units::Game> CollisionTile::testCollision(
      sides::SideType side,
      units::Game /*position*/) const {
   if (tile_type_[tiles::WALL]) {
      if (side == TOP_SIDE)
         return units::tileToGame(row_);
      if (side == BOTTOM_SIDE)
         return units::tileToGame(row_ + 1);
      if (side == LEFT_SIDE)
         return units::tileToGame(col_);
      return units::tileToGame(col_ + 1);
   }
   return boost::none;
}
