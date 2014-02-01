#ifndef ANIMATED_SPRITE_H_
#define ANIMATED_SPRITE_H_

#include "sprite.h"
#include "timer.h"

struct AnimatedSprite : public Sprite {
   AnimatedSprite(
         Graphics& graphics,
         const std::string& file_name,
         units::Pixel source_x, units::Pixel source_y,
         units::Pixel width, units::Pixel height,
         units::FPS fps, units::Frame num_frames);

   void update(units::MS elapsed_time_ms);

  private:
   Timer frame_timer_;
   const units::Frame num_frames_;
   units::Frame current_frame_;
};

#endif // ANIMATED_SPRITE_H_
