#ifndef PICKUPS_H_
#define PICKUPS_H_

#include <boost/shared_ptr.hpp>
#include <set>

#include "units.h"

struct Graphics;
struct Map;
struct Pickup;
struct Player;

struct Pickups {
   void add(boost::shared_ptr<Pickup> pickup) { pickups_.insert(pickup); }

   void handleCollision(Player& player);

   void update(units::MS elapsed_time, const Map& map);
   void draw(Graphics& graphics);
  private:
   typedef std::set<boost::shared_ptr<Pickup> > PickupSet;
   PickupSet pickups_;
};

#endif // PICKUPS_H_