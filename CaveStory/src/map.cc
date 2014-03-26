#include "map.h"

#include "game.h"
#include "sprite.h"
#include "graphics.h"
#include "rectangle.h"

using namespace tiles;

using boost::shared_ptr;
using std::vector;

//static
Map* Map::createSlopeTestMap(Graphics& graphics) {
   Map* map = new Map();

   map->backdrop_.reset(new FixedBackdrop("bkBlue", graphics));
   const units::Tile num_rows = 15;
   const units::Tile num_cols = 20;
   // Ensure tiles_ and background_tiles_ are num_rows x num_cols in size
   map->tiles_ = vector<vector<Tile> >(
         num_rows, vector<Tile>(
            num_cols, Tile()));
   map->background_tiles_ = vector<vector<shared_ptr<Sprite> > >(
      num_rows, vector<shared_ptr<Sprite> >(
      num_cols, shared_ptr<Sprite>()));

   Tile wall_tile(
         TileType().set(WALL),
         shared_ptr<Sprite>(new Sprite(
                  graphics,
                  "PrtCave",
                  units::tileToPixel(1), 0,
                  units::tileToPixel(1), units::tileToPixel(1))));
   enum {
      LTT,
      LTS,
      RTS,
      RTT,
      LBT,
      LBS,
      RBS,
      RBT,
      NUM_SLOPES
   };
   Tile slope_tiles[NUM_SLOPES];
   for (int i = 0; i < NUM_SLOPES; ++i) {
      slope_tiles[i] = Tile(
            TileType().set(SLOPE).
               set(i / 2 % 2 == 0 ? LEFT_SLOPE : RIGHT_SLOPE).
               set(i / 4 == 0 ? TOP_SLOPE : BOTTOM_SLOPE).
               set((i + 1) / 2 % 2 == 0 ? TALL_SLOPE : SHORT_SLOPE),
            shared_ptr<Sprite>(new Sprite(
                  graphics, "PrtCave",
                  units::tileToPixel(2 + i % 4), units::tileToPixel(i / 4),
                  units::tileToPixel(1), units::tileToPixel(1))));
   }
   units::Tile row = 11;
   for (units::Tile col = 0; col < num_cols; ++col) {
      map->tiles_[row][col] = wall_tile;
   }
   --row;
   units::Tile col = 0;
   map->tiles_[row][col++] = wall_tile;
   map->tiles_[row][col++] = slope_tiles[LBT];
   map->tiles_[row][col++] = slope_tiles[RBT];
   map->tiles_[row][col++] = wall_tile;
   map->tiles_[row][col++] = slope_tiles[LBS];
   ++col;
   map->tiles_[row][col++] = slope_tiles[RBS];
   map->tiles_[row][col++] = slope_tiles[RBT];
   map->tiles_[row][col++] = wall_tile;
   map->tiles_[row][col++] = slope_tiles[LBT];
   map->tiles_[row][col++] = slope_tiles[LBS];
   map->tiles_[row][col++] = slope_tiles[RBS];
   map->tiles_[row][col++] = slope_tiles[RBT];
   map->tiles_[row][col] = wall_tile;
   map->tiles_[row-1][col++] = slope_tiles[RBS];
   map->tiles_[row][col] = wall_tile;
   map->tiles_[row-1][col++] = slope_tiles[RBT];
   map->tiles_[row-1][col++] = wall_tile;
   ++col;
   map->tiles_[row][col++] = slope_tiles[RBS];
   map->tiles_[row][col++] = wall_tile;
   col = 0;
   row -= 3;
   map->tiles_[row-1][col] = wall_tile;
   map->tiles_[row][col++] = wall_tile;
   map->tiles_[row-1][col] = wall_tile;
   map->tiles_[row][col++] = slope_tiles[LTT];
   map->tiles_[row-1][col] = wall_tile;
   map->tiles_[row][col++] = slope_tiles[LTS];
   map->tiles_[row-1][col] = wall_tile;
   map->tiles_[row][col++] = slope_tiles[RTS];
   map->tiles_[row-1][col] = wall_tile;
   map->tiles_[row][col++] = slope_tiles[RTT];
   map->tiles_[row-1][col] = wall_tile;
   map->tiles_[row][col++] = wall_tile;

   return map;
}

//static
Map* Map::createTestMap(Graphics& graphics) {
   Map* map = new Map();

   map->backdrop_.reset(new FixedBackdrop("bkBlue", graphics));
   const units::Tile num_rows = 15;
   const units::Tile num_cols = 20;
   // Ensure tiles_ and background_tiles_ are num_rows x num_cols in size
   map->tiles_ = vector<vector<Tile> >(
         num_rows, vector<Tile>(
            num_cols, Tile()));
   map->background_tiles_ = vector<vector<shared_ptr<Sprite> > >(
      num_rows, vector<shared_ptr<Sprite> >(
      num_cols, shared_ptr<Sprite>()));

   shared_ptr<Sprite> sprite(new Sprite(
            graphics,
            "PrtCave",
            units::tileToPixel(1), 0,
            units::tileToPixel(1), units::tileToPixel(1)));
   Tile tile(TileType().set(WALL), sprite);
   const units::Tile row = 11;
   for (units::Tile col = 0; col < num_cols; ++col) {
      map->tiles_[row][col] = tile;
   }
   map->tiles_[10][5] = tile;
   map->tiles_[9][4] = tile;
   map->tiles_[8][3] = tile;
   map->tiles_[7][2] = tile;
   map->tiles_[10][3] = tile;

   shared_ptr<Sprite> chain_top(new Sprite(
      graphics,
      "PrtCave",
      units::tileToPixel(11), units::tileToPixel(2),
      units::tileToPixel(1), units::tileToPixel(1)));
   shared_ptr<Sprite> chain_middle(new Sprite(
      graphics,
      "PrtCave",
      units::tileToPixel(12), units::tileToPixel(2),
      units::tileToPixel(1), units::tileToPixel(1)));
   shared_ptr<Sprite> chain_bottom(new Sprite(
      graphics,
      "PrtCave",
      units::tileToPixel(13), units::tileToPixel(2),
      units::tileToPixel(1), units::tileToPixel(1)));

   map->background_tiles_[8][2] = chain_top;
   map->background_tiles_[9][2] = chain_middle;
   map->background_tiles_[10][2] = chain_bottom;

   return map;
}

vector<CollisionTile> Map::getCollidingTiles(
      const Rectangle& rectangle,
      sides::SideType direction) const {
   const units::Tile first_primary =
      units::gameToTile(rectangle.side(sides::opposite_side(direction)));
   const units::Tile last_primary =
      units::gameToTile(rectangle.side(direction));
   const units::Tile primary_incr =
      direction == sides::BOTTOM_SIDE || direction == sides::RIGHT_SIDE ?
      1 : -1;

   const bool horizontal = sides::horizontal(direction);
   const units::Tile s_min =
      units::gameToTile(horizontal ? rectangle.top() : rectangle.left());
   const units::Tile s_mid =
      units::gameToTile(horizontal ? rectangle.center_y() : rectangle.center_x());
   const units::Tile s_max =
      units::gameToTile(horizontal ? rectangle.bottom() : rectangle.right());

   const bool s_positive = s_mid - s_min < s_max - s_mid;
   const units::Tile secondary_incr = s_positive ? 1 : -1;
   const units::Tile first_secondary = s_positive ? s_min : s_max;
   const units::Tile last_secondary = !s_positive ? s_min : s_max;

   vector<CollisionTile> collision_tiles;
   for (units::Tile primary = first_primary;
        primary != last_primary + primary_incr;
        primary += primary_incr) {
      for (units::Tile secondary = first_secondary;
           secondary != last_secondary + secondary_incr;
           secondary += secondary_incr) {
         const units::Tile row = !horizontal ? primary : secondary;
         const units::Tile col = horizontal ? primary : secondary;
         collision_tiles.push_back(CollisionTile(row, col, tiles_[row][col].tile_type));
      }
   }
   return collision_tiles;   
}

void Map::drawBackground(Graphics& graphics) const {
   backdrop_->draw(graphics);
   for (size_t row = 0; row < background_tiles_.size(); ++row) {
      for (size_t col = 0; col < background_tiles_[row].size(); ++col) {
         if (background_tiles_[row][col]) {
            background_tiles_[row][col]->draw(graphics, units::tileToGame(col), units::tileToGame(row));
         }
      }
   }
}

void Map::draw(Graphics& graphics) const {
   for (size_t row = 0; row < tiles_.size(); ++row) {
      for (size_t col = 0; col < tiles_[row].size(); ++col) {
         if (tiles_[row][col].sprite) {
            tiles_[row][col].sprite->draw(
               graphics, units::tileToGame(col), units::tileToGame(row));
         }
      }
   }
}
