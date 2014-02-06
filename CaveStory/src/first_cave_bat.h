#ifndef FIRST_CAVE_BAT_H_
#define FIRST_CAVE_BAT_H_

#include "sprite_state.h"

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
   Rectangle collisionRectangle() const
      { return Rectangle(x_, y_, units::tileToGame(1), units::tileToGame(1)); }

   units::HP contactDamage() const;
   void takeDamage(units::HP damage)
    { printf("%d! collision occurred\n", damage); }

  private:
   typedef boost::tuple<HorizontalFacing> SpriteTuple;
   struct SpriteState : public SpriteTuple {
      SpriteState(const SpriteTuple& tuple) : SpriteTuple(tuple) {}
      HorizontalFacing horizontal_facing() const { return get<0>(); }
   };

   void initializeSprites(Graphics& graphics);
   void initializeSprite(Graphics& graphics, const SpriteState& sprite_state);
   SpriteState getSpriteState() const;

   const units::Game center_y_;
   units::Game x_, y_;
   units::Degrees flight_angle_;
   HorizontalFacing facing_;
   std::map<SpriteState, boost::shared_ptr<Sprite> > sprites_;
};

#endif // FIRST_CAVE_BAT_H_
