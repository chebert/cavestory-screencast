#ifndef GAME_H_
#define GAME_H_

#include <boost/scoped_ptr.hpp>

#include "units.h"

struct Map;
struct Player;
struct FirstCaveBat;
struct Graphics;

struct Game {
   Game();
   ~Game();

   static units::Tile kScreenWidth;
   static units::Tile kScreenHeight;
  private:
   void eventLoop();
   void update(units::MS elapsed_time_ms);
   void draw(Graphics& graphics);

   boost::scoped_ptr<Player> player_;
   boost::scoped_ptr<FirstCaveBat> bat_;
   boost::scoped_ptr<Map> map_;
};

#endif // GAME_H_
