#include "first_cave_bat.h"

#include "animated_sprite.h"
#include "graphics.h"

namespace {
const units::Frame kNumFlyFrames = 3;
const units::FPS kFlyFps = 13;

const units::AngularVelocity kAngularVelocity = 120.0f / 1000.0f;

const units::Game kFlightAmplitude = 5 * units::kHalfTile;
const units::HP kContactDamage = 1;
}

FirstCaveBat::FirstCaveBat(Graphics& graphics, units::Game x, units::Game y) :
   flight_center_y_(y),
   x_(x),
   y_(y),
   flight_angle_(0.0f),
   facing_(RIGHT)
{
   initializeSprites(graphics);
}

void FirstCaveBat::draw(Graphics& graphics) {
   sprites_.at(getSpriteState())->draw(graphics, x_, y_);
   damage_text_.draw(graphics, center_x(), center_y());
}

void FirstCaveBat::update(units::MS elapsed_time, units::Game player_x) {
   flight_angle_ += kAngularVelocity * elapsed_time;
   damage_text_.update(elapsed_time);

   facing_ = x_ + units::kHalfTile > player_x ?
      LEFT :
      RIGHT;

   y_ = flight_center_y_ +
      kFlightAmplitude * units::Game(std::sin(units::degreesToRadians(flight_angle_)));

   sprites_[getSpriteState()]->update();
}

units::HP FirstCaveBat::contactDamage() const {
   return kContactDamage;
}

void FirstCaveBat::initializeSprites(Graphics& graphics) {
   ENUM_FOREACH(h_facing, HORIZONTAL_FACING) {
      initializeSprite(graphics, boost::make_tuple(HorizontalFacing(h_facing)));
   }
}

void FirstCaveBat::initializeSprite(Graphics& graphics, const SpriteState& sprite_state) {
   units::Tile tile_y = sprite_state.horizontal_facing() == RIGHT ? 3 : 2;
   sprites_[sprite_state] = boost::shared_ptr<Sprite>(new AnimatedSprite(
      graphics, "NpcCemet",
      units::tileToPixel(2), units::tileToPixel(tile_y),
      units::tileToPixel(1), units::tileToPixel(1),
      kFlyFps, kNumFlyFrames));
}

FirstCaveBat::SpriteState FirstCaveBat::getSpriteState() const {
   return boost::make_tuple(facing_);
}
