#include "floating_number.h"

#include "number_sprite.h"

namespace {
const units::Velocity kVelocity = -units::kHalfTile / 250;
const units::MS kDamageTime = 2000;
}

FloatingNumber::FloatingNumber(NumberType type) :
   should_rise_(true),
   center_x_(0), center_y_(0),
   value_(0), offset_y_(0),
   timer_(kDamageTime),
   type_(type) {}

void FloatingNumber::addValue(int value) {
   should_rise_ = value_ == 0;
   if (should_rise_) {
      offset_y_ = 0;
   }
   value_ += value;
   timer_.reset();
}

bool FloatingNumber::update(units::MS elapsed_time) {
   if (timer_.expired()) {
      value_ = 0;
   } else if (should_rise_) {
      offset_y_ = std::max(-units::tileToGame(1), offset_y_ + kVelocity * elapsed_time);
   }
   return !timer_.expired();
}

void FloatingNumber::setPosition(units::Game center_x, units::Game center_y) {
   center_x_ = center_x;
   center_y_ = center_y;
}


void FloatingNumber::draw(Graphics& graphics) {
   if (timer_.expired()) return;
   if (type_ == DAMAGE) {
      NumberSprite::DamageNumber(graphics, value_).
         drawCentered(graphics, center_x_, center_y_ + offset_y_);
   } else {
      NumberSprite::ExperienceNumber(graphics, value_).
         drawCentered(graphics, center_x_, center_y_ + offset_y_);
   }
}
