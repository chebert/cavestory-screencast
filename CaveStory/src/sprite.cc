#include "sprite.h"

#include "graphics.h"

Sprite::Sprite(
      Graphics& graphics,
      const std::string& file_path,
      int source_x, int source_y,
      int width, int height) {
   sprite_sheet_ = graphics.loadImage(file_path);
   source_rect_.x = source_x;
   source_rect_.y = source_y;
   source_rect_.w = width;
   source_rect_.h = height;
}

void Sprite::draw(Graphics& graphics, int x, int y) {
   SDL_Rect destination_rectangle;
   destination_rectangle.x = x;
   destination_rectangle.y = y;
   graphics.blitSurface(sprite_sheet_, &source_rect_, &destination_rectangle);
}
