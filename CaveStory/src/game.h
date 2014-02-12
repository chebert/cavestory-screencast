#ifndef GAME_H_
#define GAME_H_

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "units.h"
#include "damage_texts.h"

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

   boost::shared_ptr<Player> player_;
   boost::shared_ptr<FirstCaveBat> bat_;
   boost::scoped_ptr<Map> map_;
   DamageTexts damage_texts_;
};

#endif // GAME_H_
