#include "animated_sprite.h"

#include "game.h"

AnimatedSprite::AnimatedSprite(
      Graphics& graphics,
      const std::string& file_path,
      units::Pixel source_x, units::Pixel source_y,
      units::Pixel width, units::Pixel height,
      units::FPS fps, units::Frame num_frames) :
   Sprite(graphics, file_path, source_x, source_y, width, height),
   frame_timer_(1000 / fps),
   num_frames_(num_frames),
   current_frame_(0)
{
}

void AnimatedSprite::update(units::MS elapsed_time_ms) {
   if (frame_timer_.expired()) {
      ++current_frame_;
      frame_timer_.reset();
      if (current_frame_ < num_frames_) {
         source_rect_.x += source_rect_.w;
      } else {
         source_rect_.x -= source_rect_.w * (num_frames_ - 1);
         current_frame_ = 0;
      }
   }
}
