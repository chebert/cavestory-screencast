#include "flashing_pickup.h"

#include <string>

const units::MS kLifeTime = 8000;
const units::MS kFlashPeriod = 60;
const units::MS kFlickerTime = kLifeTime - 1000;
const units::MS kFlickerPeriod = 50;
const units::MS kDissipateTime = kLifeTime - 25;

const units::Tile kDissipatingSourceX = 1;
const units::Tile kDissipatingSourceY = 0;

const units::Tile kHeartSourceX = 2;
const units::Tile kHeartSourceY = 5;
const Rectangle kHeartRectangle(5, 8, 21, 19);
const units::HP kHeartValue = 2;

const units::Tile kMultiHeartSourceX = 4;
const units::Tile kMultiHeartSourceY = 5;
const Rectangle kMultiHeartRectangle(6, 7, 26, 25);
const units::HP kMultiHeartValue = 6;

const std::string kSpriteName("NpcSym");

Rectangle FlashingPickup::collisionRectangle() const {
   return Rectangle(
         x_ + rectangle_.left(),
         y_ + rectangle_.top(),
         rectangle_.width(),
         rectangle_.height());
}

void FlashingPickup::draw(Graphics& graphics) {
   units::MS time = timer_.current_time();
   if (time > kDissipateTime) {
      dissipating_sprite_.draw(graphics, x_, y_);
   } else if (time < kFlickerTime || time / kFlickerPeriod % 2 == 0) {
      if (time / kFlashPeriod % 2 == 0) {
         sprite_.draw(graphics, x_, y_);
      } else {
         flash_sprite_.draw(graphics, x_, y_);
      }
   }
}

bool FlashingPickup::update(units::MS, const Map&) {
   return timer_.active();
}

//static
boost::shared_ptr<Pickup> FlashingPickup::heartPickup(
      Graphics& graphics,
      units::Game center_x, units::Game center_y) {
   return boost::shared_ptr<Pickup>(new FlashingPickup(
            graphics,
            center_x, center_y,
            kHeartSourceX, kHeartSourceY,
            kHeartRectangle,
            kHeartValue,
            Pickup::HEALTH));
}

//static
boost::shared_ptr<Pickup> FlashingPickup::multiHeartPickup(
      Graphics& graphics,
      units::Game center_x, units::Game center_y) {
   return boost::shared_ptr<Pickup>(new FlashingPickup(
            graphics,
            center_x, center_y,
            kMultiHeartSourceX, kMultiHeartSourceY,
            kMultiHeartRectangle,
            kMultiHeartValue,
            Pickup::HEALTH));
}

FlashingPickup::FlashingPickup(
      Graphics& graphics,
      units::Game center_x, units::Game center_y,
      units::Tile source_x, units::Tile source_y, 
      const Rectangle& rectangle,
      const int value,
      const PickupType type) :
   sprite_(graphics, kSpriteName,
         units::tileToPixel(source_x),
         units::tileToPixel(source_y),
         units::tileToPixel(1), units::tileToPixel(1)),
   flash_sprite_(graphics, kSpriteName,
         units::tileToPixel(source_x + 1),
         units::tileToPixel(source_y),
         units::tileToPixel(1), units::tileToPixel(1)),
   dissipating_sprite_(graphics, kSpriteName,
         units::tileToPixel(kDissipatingSourceX),
         units::tileToPixel(kDissipatingSourceY),
         units::tileToPixel(1), units::tileToPixel(1)),
   x_(center_x - units::kHalfTile), y_(center_y - units::kHalfTile),
   timer_(kLifeTime, true),
   rectangle_(rectangle),
   value_(value),
   type_(type) {}
