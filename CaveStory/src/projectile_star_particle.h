#ifndef PROJECTILE_STAR_PARTICLE_H_
#define PROJECTILE_STAR_PARTICLE_H_

#include "particle.h"
#include "animated_sprite.h"

struct ProjectileStarParticle : public Particle {
   ProjectileStarParticle(Graphics& graphics, units::Game x, units::Game y);

   bool update(units::MS elapsed_time);
   void draw(Graphics& graphics);
  private:
   const units::Game x_, y_;
   AnimatedSprite sprite_;
};

#endif // PROJECTILE_STAR_PARTICLE_H_