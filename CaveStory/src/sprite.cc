#include "sprite.h"

#include "graphics.h"

Sprite::Sprite(
      Graphics& graphics,
      const std::string& file_name,
      units::Pixel source_x, units::Pixel source_y,
      units::Pixel width, units::Pixel height) {
   const bool black_is_transparent = true;
   sprite_sheet_ = graphics.loadImage(file_name, black_is_transparent);
   source_rect_.x = source_x;
   source_rect_.y = source_y;
   source_rect_.w = width;
   source_rect_.h = height;
}

void Sprite::draw(Graphics& graphics, units::Game x, units::Game y) {
   SDL_Rect destination_rectangle;
   destination_rectangle.x = units::gameToPixel(x);
   destination_rectangle.y = units::gameToPixel(y);
   graphics.blitSurface(sprite_sheet_, &source_rect_, &destination_rectangle);
}
