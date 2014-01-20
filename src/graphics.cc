#include "graphics.h"

#include <SDL/SDL.h>

namespace {
const int kScreenWidth = 640;
const int kScreenHeight = 480;
const int kBitsPerPixel = 32;
}

Graphics::Graphics() {
   screen_ = SDL_SetVideoMode(
         kScreenWidth,
         kScreenHeight,
         kBitsPerPixel,
         SDL_FULLSCREEN);
}

Graphics::~Graphics() {
   for (SpriteMap::iterator iter = sprite_sheets_.begin();
        iter != sprite_sheets_.end();
        ++iter) {
      SDL_FreeSurface(iter->second);
   }
   SDL_FreeSurface(screen_);
}

Graphics::SurfaceID Graphics::loadImage(const std::string& file_path) {
   // if we have not loaded in the spritesheet
   if (sprite_sheets_.count(file_path) == 0) {
      // load it in now
      sprite_sheets_[file_path] = SDL_LoadBMP(file_path.c_str());
   }
   return sprite_sheets_[file_path];
}

void Graphics::blitSurface(
      SurfaceID source,
      SDL_Rect* source_rectangle,
      SDL_Rect* destination_rectangle) {
   SDL_BlitSurface(source, source_rectangle, screen_, destination_rectangle);
}

void Graphics::clear() {
   SDL_FillRect(screen_, NULL/*destination_rectangle*/, 0/*color*/);
}

void Graphics::flip() {
   SDL_Flip(screen_);
}
