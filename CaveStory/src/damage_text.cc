#include "damage_text.h"

#include "number_sprite.h"

namespace {
const units::Velocity kVelocity = -units::kHalfTile / 250;
const units::MS kDamageTime = 2000;
}

DamageText::DamageText() : should_rise_(true), damage_(0), offset_y_(0), timer_(kDamageTime) {}

void DamageText::setDamage(units::HP damage) {
   should_rise_ = damage_ == 0;
   if (should_rise_) {
      offset_y_ = 0;
   }
   damage_ += damage;
   timer_.reset();
}

void DamageText::update(units::MS elapsed_time) {
   if (timer_.expired()) {
      damage_ = 0;
   } else if (should_rise_) {
      offset_y_ = std::max(-units::tileToGame(1), offset_y_ + kVelocity * elapsed_time);
   }
}

void DamageText::draw(Graphics& graphics, units::Game center_x, units::Game center_y) {
   if (timer_.expired()) return;
   NumberSprite::DamageNumber(graphics, damage_).
      drawCentered(graphics, center_x, center_y + offset_y_);
}