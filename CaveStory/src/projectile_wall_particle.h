#ifndef PROJECTILE_WALL_PARTICLE_H_
#define PROJECTILE_WALL_PARTICLE_H_

#include "immobile_single_loop_particle.h"

struct ProjectileWallParticle : public ImmobileSingleLoopParticle {
   ProjectileWallParticle(Graphics& graphics, units::Game x, units::Game y);
};

#endif // PROJECTILE_WALL_PARTICLE_H_