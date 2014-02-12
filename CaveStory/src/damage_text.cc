#include "damage_text.h"

#include "number_sprite.h"

namespace {
const units::Velocity kVelocity = -units::kHalfTile / 250;
const units::MS kDamageTime = 2000;
}

DamageText::DamageText() : should_rise_(true), center_x_(0), center_y_(0), damage_(0), offset_y_(0), timer_(kDamageTime) {}

void DamageText::setDamage(units::HP damage) {
   should_rise_ = damage_ == 0;
   if (should_rise_) {
      offset_y_ = 0;
   }
   damage_ += damage;
   timer_.reset();
}

bool DamageText::update(units::MS elapsed_time) {
   if (timer_.expired()) {
      damage_ = 0;
   } else if (should_rise_) {
      offset_y_ = std::max(-units::tileToGame(1), offset_y_ + kVelocity * elapsed_time);
   }
   return !timer_.expired();
}

void DamageText::setPosition(units::Game center_x, units::Game center_y) {
   center_x_ = center_x;
   center_y_ = center_y;
}


void DamageText::draw(Graphics& graphics) {
   if (timer_.expired()) return;
   NumberSprite::DamageNumber(graphics, damage_).
      drawCentered(graphics, center_x_, center_y_ + offset_y_);
}