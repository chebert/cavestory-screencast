#include "map.h"

#include "game.h"
#include "sprite.h"
#include "graphics.h"

using boost::shared_ptr;
using std::vector;

//static
Map* Map::createTestMap(Graphics& graphics) {
   Map* map = new Map();

   const int num_rows = 15;
   const int num_cols = 20;
   // Ensure foreground_sprites_ is num_rows x num_cols in size
   map->foreground_sprites_ = vector<vector<shared_ptr<Sprite> > >(
         num_rows, vector<shared_ptr<Sprite> >(
            num_cols, shared_ptr<Sprite>()));

   shared_ptr<Sprite> sprite(new Sprite(
            graphics,
            "content/PrtCave.bmp",
            Game::kTileSize, 0,
            Game::kTileSize, Game::kTileSize));
   const int row = 11;
   for (int col = 0; col < num_cols; ++col) {
      map->foreground_sprites_[row][col] = sprite;
   }
   map->foreground_sprites_[10][5] = sprite;
   map->foreground_sprites_[9][4] = sprite;
   map->foreground_sprites_[8][3] = sprite;
   map->foreground_sprites_[7][2] = sprite;
   map->foreground_sprites_[10][3] = sprite;

   return map;
}

void Map::update(int elapsed_time_ms) {
   for (size_t row = 0; row < foreground_sprites_.size(); ++row) {
      for (size_t col = 0; col < foreground_sprites_[row].size(); ++col) {
         if (foreground_sprites_[row][col]) {
            foreground_sprites_[row][col]->update(elapsed_time_ms);
         }
      }
   }
}

void Map::draw(Graphics& graphics) const {
   for (size_t row = 0; row < foreground_sprites_.size(); ++row) {
      for (size_t col = 0; col < foreground_sprites_[row].size(); ++col) {
         if (foreground_sprites_[row][col]) {
            foreground_sprites_[row][col]->draw(
                  graphics, col*Game::kTileSize, row*Game::kTileSize);
         }
      }
   }
}
