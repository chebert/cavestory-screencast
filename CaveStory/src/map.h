#ifndef MAP_H_
#define MAP_H_

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

#include "backdrop.h"
#include "collision_tile.h"
#include "tile_type.h"
#include "units.h"

struct Graphics;
struct Sprite;
struct Rectangle;

struct Map {
   static Map* createTestMap(Graphics& graphics);

   std::vector<CollisionTile> getCollidingTiles(const Rectangle& rectangle) const;

   void drawBackground(Graphics& graphics) const;
   void draw(Graphics& graphics) const;

  private:
   struct Tile {
      Tile(tiles::TileType tile_type = tiles::AIR_TILE,
           boost::shared_ptr<Sprite> sprite = boost::shared_ptr<Sprite>()) :
         tile_type(tile_type),
         sprite(sprite) {}
      tiles::TileType tile_type;
      boost::shared_ptr<Sprite> sprite;
   };
   boost::scoped_ptr<Backdrop> backdrop_;
   std::vector<std::vector<boost::shared_ptr<Sprite> > > background_tiles_;
   std::vector<std::vector<Tile> > tiles_;
};

#endif // MAP_H_
