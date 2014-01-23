#include "number_sprite.h"

#include <assert.h>

#include "sprite.h"

namespace {

const std::string kSpritePath("../content/TextBox.bmp");
const units::Game kSourceY = 7 * units::kHalfTile;

const units::Game kSourceWidth = units::kHalfTile;
const units::Game kSourceHeight = units::kHalfTile;

}

NumberSprite::NumberSprite(Graphics& graphics, int number, int num_digits) :
   padding_(0.0f)
{
   assert(number >= 0);

   int digit_count = 0;
   do {
      const int digit = number % 10;
      reversed_digits_.push_back(boost::shared_ptr<Sprite>(new Sprite(
         graphics, kSpritePath,
         units::gameToPixel(digit*units::kHalfTile), units::gameToPixel(kSourceY),
         units::gameToPixel(kSourceWidth), units::gameToPixel(kSourceHeight))));
      number /= 10;
      ++digit_count;
   } while (number != 0);
   // if num_digits is 0, no shifting
   // if num_digits is digit_count, no shifting
   assert(num_digits == 0 || num_digits >= digit_count);
   padding_ = num_digits == 0 ? 0.0f : units::kHalfTile * (num_digits - digit_count);
}

void NumberSprite::draw(Graphics& graphics, units::Game x, units::Game y) {
   for (size_t i = 0; i < reversed_digits_.size(); ++i) {
      // when i == reversed_digits_.size() - 1, offset = 0
      // when 0 == reversed_digits_.size() - 1 - i, offset = 0
      const units::Game offset = units::kHalfTile * (reversed_digits_.size() - 1 - i);
      reversed_digits_[i]->draw(graphics, x + offset + padding_, y);
   }
}