#ifndef DEATH_CLOUD_PARTICLE_H_
#define DEATH_CLOUD_PARTICLE_H_

#include "animated_sprite.h"
#include "particle.h"
#include "polar_vector.h"

struct ParticleTools;

struct DeathCloudParticle : public Particle {
   DeathCloudParticle(Graphics& graphics,
      units::Game center_x, units::Game center_y,
      units::Velocity speed, units::Degrees angle);

   bool update(units::MS elapsed_time);
   void draw(Graphics& graphics);

   static void createRandomDeathClouds(
      ParticleTools& particle_tools,
      units::Game center_x, units::Game center_y,
      int num_particles);

  private:
   const units::Game center_x_, center_y_;
   const units::Velocity speed_;
   PolarVector offset_;
   AnimatedSprite sprite_;
};

#endif // DEATH_CLOUD_PARTICLE_H_