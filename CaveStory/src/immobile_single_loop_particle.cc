#include "immobile_single_loop_particle.h"

ImmobileSingleLoopParticle::ImmobileSingleLoopParticle(Graphics& graphics,
      const std::string& sprite_name, 
      units::Pixel source_x, units::Pixel source_y,
      units::Pixel source_width, units::Pixel source_height,
      units::FPS fps, units::Frame num_frames,
      units::Game x, units::Game y) :
   x_(x), y_(y),
   sprite_(graphics, sprite_name,
      source_x, source_y, source_width, source_height,
      fps, num_frames) {}

bool ImmobileSingleLoopParticle::update(units::MS /*elapsed_time*/) {
   sprite_.update();
   return sprite_.num_completed_loops() == 0;
}

void ImmobileSingleLoopParticle::draw(Graphics& graphics) {
   sprite_.draw(graphics, x_, y_);
}