#ifndef POLAR_STAR_H_
#define POLAR_STAR_H_

#include "sprite_state.h"

#include "units.h"

struct Graphics;
struct Sprite;

struct PolarStar {
   PolarStar(Graphics& graphics);

   void draw(
      Graphics& graphics,
      HorizontalFacing horizontal_facing, VerticalFacing vertical_facing,
      bool gun_up,
      units::Game x, units::Game y);
  private:
   typedef boost::tuple<HorizontalFacing, VerticalFacing> SpriteTuple;
   struct SpriteState : public SpriteTuple {
      SpriteState(const SpriteTuple& tuple) : SpriteTuple(tuple) {}
      HorizontalFacing horizontal_facing() const { return get<0>(); }
      VerticalFacing vertical_facing() const { return get<1>(); }
   };

   void initializeSprites(Graphics& graphics);
   void initializeSprite(Graphics& graphics, const SpriteState& sprite_state);

   std::map<SpriteState, boost::shared_ptr<Sprite> > sprite_map_;
   boost::shared_ptr<Sprite> horizontal_projectile_;
   boost::shared_ptr<Sprite> vertical_projectile_;
};

#endif // POLAR_STAR_H_
