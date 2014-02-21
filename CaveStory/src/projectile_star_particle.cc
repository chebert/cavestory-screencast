#include "projectile_star_particle.h"

const std::string kSpriteName("Caret");
const units::Tile kSourceX = 0;
const units::Tile kSourceY = 3;
const units::Tile kSourceWidth = 1;
const units::Tile kSourceHeight = 1;
const units::FPS kFps = 18;
const units::Frame kNumFrames = 4;

ProjectileStarParticle::ProjectileStarParticle(Graphics& graphics, units::Game x, units::Game y) :
   x_(x), y_(y),
   sprite_(graphics, kSpriteName,
      units::tileToPixel(kSourceX), units::tileToPixel(kSourceY), 
      units::tileToPixel(kSourceWidth), units::tileToPixel(kSourceHeight), 
      kFps, kNumFrames) {}

bool ProjectileStarParticle::update(units::MS /*elapsed_time*/) {
   sprite_.update();
   return sprite_.num_completed_loops() == 0;
}

void ProjectileStarParticle::draw(Graphics& graphics) {
   sprite_.draw(graphics, x_, y_);
}