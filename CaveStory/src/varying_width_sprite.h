#ifndef VARYING_WIDTH_SPRITE_H_
#define VARYING_WIDTH_SPRITE_H_

#include "sprite.h"

struct VaryingWidthSprite : public Sprite {
   VaryingWidthSprite(Graphics& graphics,
          const std::string& file_path,
          units::Pixel source_x, units::Pixel source_y,
          units::Pixel initial_width, units::Pixel height) :
      Sprite(graphics, file_path, source_x, source_y, initial_width, height) {}

   void set_width(units::Pixel width) { source_rect_.w = width; }
};

#endif // VARYING_WIDTH_SPRITE_H_