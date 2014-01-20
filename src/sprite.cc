#include "sprite.h"

#include "graphics.h"

Sprite::Sprite(
      const std::string& file_path,
      int source_x, int source_y,
      int width, int height) {
   sprite_sheet_ = SDL_LoadBMP(file_path.c_str());
   source_rect_.x = source_x;
   source_rect_.y = source_y;
   source_rect_.w = width;
   source_rect_.h = height;
}

Sprite::~Sprite() {
   SDL_FreeSurface(sprite_sheet_);
}

void Sprite::draw(Graphics& graphics, int x, int y) {
   SDL_Rect destination_rectangle;
   destination_rectangle.x = x;
   destination_rectangle.y = y;
   graphics.blitSurface(sprite_sheet_, &source_rect_, &destination_rectangle);
}
