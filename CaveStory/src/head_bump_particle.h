#ifndef HEAD_BUMP_PARTICLE_H_
#define HEAD_BUMP_PARTICLE_H_

#include "polar_vector.h"
#include "sprite.h"
#include "timer.h"
#include "units.h"

struct Graphics;

struct HeadBumpParticle {
   HeadBumpParticle(Graphics& graphics, units::Game center_x, units::Game center_y);
   void draw(Graphics& graphics);
   bool update(units::MS elapsed_time);

  private:
   const units::Game center_x_, center_y_;
   Sprite sprite_;
   Timer timer_; // Measures how much time we have to live.
   PolarVector particle_a_, particle_b_;
   const units::Game max_offset_a_, max_offset_b_;
};

#endif // HEAD_BUMP_PARTICLE_H_