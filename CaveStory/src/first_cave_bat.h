#ifndef FIRST_CAVE_BAT_H_
#define FIRST_CAVE_BAT_H_

#include <boost/shared_ptr.hpp>
#include <map>
#include "units.h"
#include "rectangle.h"

struct Graphics;
struct Sprite;

struct FirstCaveBat {
   FirstCaveBat(Graphics& graphics, units::Game x, units::Game y);

   void draw(Graphics& graphics) const;
   void update(units::MS elapsed_time, units::Game player_x);

   Rectangle damageRectangle() const
      { return Rectangle(x_ + units::kHalfTile, y_ + units::kHalfTile, 0, 0); }

   units::HP contactDamage() const;

  private:
   enum Facing {
      FIRST_FACING,
      LEFT = FIRST_FACING,
      RIGHT,
      LAST_FACING
   };
   struct SpriteState {
      SpriteState(Facing facing) : facing(facing) {}
      Facing facing;
   };
   friend bool operator<(const SpriteState& a, const SpriteState& b)
      { return a.facing < b.facing; }

   void initializeSprites(Graphics& graphics);
   void initializeSprite(Graphics& graphics, const SpriteState& sprite_state);
   SpriteState getSpriteState() const;

   const units::Game center_y_;
   units::Game x_, y_;
   units::Degrees flight_angle_;
   Facing facing_;
   std::map<SpriteState, boost::shared_ptr<Sprite> > sprites_;
};

#endif // FIRST_CAVE_BAT_H_