#ifndef ANIMATED_SPRITE_H_
#define ANIMATED_SPRITE_H_

#include "sprite.h"

struct AnimatedSprite : public Sprite {
   AnimatedSprite(
         Graphics& graphics,
         const std::string& file_path,
         units::Pixel source_x, units::Pixel source_y,
         units::Pixel width, units::Pixel height,
         units::FPS fps, units::Frame num_frames);

   void update(units::MS elapsed_time_ms);

  private:
   const units::MS frame_time_;
   const units::Frame num_frames_;
   units::Frame current_frame_;
   units::MS elapsed_time_; // Elapsed time since the last frame change.
};

#endif // ANIMATED_SPRITE_H_
