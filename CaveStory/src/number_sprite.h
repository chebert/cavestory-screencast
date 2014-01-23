#ifndef NUMBER_SPRITE_H_
#define NUMBER_SPRITE_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include "units.h"

struct Graphics;
struct Sprite;

struct NumberSprite {
   // if num_digits is 0, we don't care how much space it takes up.
   NumberSprite(Graphics& graphics, int number, int num_digits=0);

   void draw(Graphics& graphics, units::Game x, units::Game y);

  private:
   units::Game padding_;
   std::vector<boost::shared_ptr<Sprite> > reversed_digits_;
};

#endif // NUMBER_SPRITE_H_