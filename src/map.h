#ifndef MAP_H_
#define MAP_H_

#include <boost/shared_ptr.hpp>
#include <vector>

struct Graphics;
struct Sprite;

struct Map {
   static Map* createTestMap(Graphics& graphics);

   void update(int elapsed_time_ms);
   void draw(Graphics& graphics) const;

  private:
   std::vector<std::vector<boost::shared_ptr<Sprite> > > foreground_sprites_;
};

#endif // MAP_H_
