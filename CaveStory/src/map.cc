#include "map.h"

#include "game.h"
#include "sprite.h"
#include "graphics.h"
#include "rectangle.h"

using boost::shared_ptr;
using std::vector;

//static
Map* Map::createTestMap(Graphics& graphics) {
   Map* map = new Map();

   map->backdrop_.reset(new FixedBackdrop("../content/bkBlue.bmp", graphics));
   const int num_rows = 15;
   const int num_cols = 20;
   // Ensure foreground_sprites_ is num_rows x num_cols in size
   map->tiles_ = vector<vector<Tile> >(
         num_rows, vector<Tile>(
            num_cols, Tile()));

   shared_ptr<Sprite> sprite(new Sprite(
            graphics,
            "../content/PrtCave.bmp",
            Game::kTileSize, 0,
            Game::kTileSize, Game::kTileSize));
   Tile tile(WALL_TILE, sprite);
   const int row = 11;
   for (int col = 0; col < num_cols; ++col) {
      map->tiles_[row][col] = tile;
   }
   map->tiles_[10][5] = tile;
   map->tiles_[9][4] = tile;
   map->tiles_[8][3] = tile;
   map->tiles_[7][2] = tile;
   map->tiles_[10][3] = tile;

   return map;
}

vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle& rectangle) const {
   const int first_row = rectangle.top() / Game::kTileSize;
   const int last_row = rectangle.bottom() / Game::kTileSize;
   const int first_col = rectangle.left() / Game::kTileSize;
   const int last_col = rectangle.right() / Game::kTileSize;
   vector<CollisionTile> collision_tiles;
   for (int row = first_row; row <= last_row; ++row) {
      for (int col = first_col; col <= last_col; ++col) {
         collision_tiles.push_back(CollisionTile(row, col, tiles_[row][col].tile_type));
      }
   }
   return collision_tiles;   
}


void Map::update(int elapsed_time_ms) {
   for (size_t row = 0; row < tiles_.size(); ++row) {
      for (size_t col = 0; col < tiles_[row].size(); ++col) {
         if (tiles_[row][col].sprite) {
            tiles_[row][col].sprite->update(elapsed_time_ms);
         }
      }
   }
}

void Map::drawBackground(Graphics& graphics) const {
   backdrop_->draw(graphics);
}

void Map::draw(Graphics& graphics) const {
   for (size_t row = 0; row < tiles_.size(); ++row) {
      for (size_t col = 0; col < tiles_[row].size(); ++col) {
         if (tiles_[row][col].sprite) {
            tiles_[row][col].sprite->draw(
                  graphics, col*Game::kTileSize, row*Game::kTileSize);
         }
      }
   }
}
