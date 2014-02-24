#include "death_cloud_particle.h"

const std::string kSpriteName("NpcSym");
const units::Tile kSourceX = 1;
const units::Tile kSourceY = 0;
const units::Tile kSourceWidth = 1;
const units::Tile kSourceHeight = 1;
const units::FPS kFps = 18;
const units::Frame kNumFrames = 7;

DeathCloudParticle::DeathCloudParticle(
   Graphics& graphics,
   units::Game center_x, units::Game center_y,
   units::Velocity speed, units::Degrees angle) :
      center_x_(center_x - units::kHalfTile),
      center_y_(center_y - units::kHalfTile),
      speed_(speed),
      offset_(0, angle),
      sprite_(graphics, kSpriteName,
         units::tileToPixel(kSourceX), units::tileToPixel(kSourceY), 
         units::tileToPixel(kSourceWidth), units::tileToPixel(kSourceHeight), 
         kFps, kNumFrames) {}

bool DeathCloudParticle::update(units::MS elapsed_time) {
   sprite_.update();
   offset_.magnitude += speed_ * elapsed_time;
   return sprite_.num_completed_loops() == 0;
}

void DeathCloudParticle::draw(Graphics& graphics) {
   sprite_.draw(graphics, center_x_ + offset_.get_x(), center_y_ + offset_.get_y());
}