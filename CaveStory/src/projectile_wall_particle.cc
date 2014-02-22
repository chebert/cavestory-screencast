#include "projectile_wall_particle.h"

const std::string kSpriteName("Caret");
const units::Tile kSourceX = 11;
const units::Tile kSourceY = 0;
const units::Tile kSourceWidth = 1;
const units::Tile kSourceHeight = 1;
const units::FPS kFps = 20;
const units::Frame kNumFrames = 4;

ProjectileWallParticle::ProjectileWallParticle(Graphics& graphics, units::Game x, units::Game y) :
   ImmobileSingleLoopParticle(graphics, kSpriteName,
      units::tileToPixel(kSourceX), units::tileToPixel(kSourceY), 
      units::tileToPixel(kSourceWidth), units::tileToPixel(kSourceHeight), 
      kFps, kNumFrames,
      x, y) {}