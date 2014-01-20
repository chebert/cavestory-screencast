#ifndef GAME_H_
#define GAME_H_

#include <boost/scoped_ptr.hpp>

struct Map;
struct Player;
struct Graphics;

struct Game {
   Game();
   ~Game();

   static int kTileSize;
   static int kScreenWidth;
   static int kScreenHeight;
   template <typename T>
   static T gameUnitsToPixels(T gameUnits);
  private:
   void eventLoop();
   void update(int elapsed_time_ms);
   void draw(Graphics& graphics);

   boost::scoped_ptr<Player> player_;
   boost::scoped_ptr<Map> map_;
};

//static
template <typename T>
T Game::gameUnitsToPixels(T gameUnits) {
   return gameUnits * Game::kTileSize / 32;
}

#endif // GAME_H_
