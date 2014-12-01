#include "graphics.h"

#ifndef _WIN32
#include <boost/filesystem.hpp>
#endif
#include <SDL.h>

#include "game.h"
#include "rectangle.h"

namespace {
const int kBitsPerPixel = 32;
}

Graphics::Graphics() {
   window_ = SDL_CreateWindow("Reconstructed Cave Story",
      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      units::tileToPixel(Game::kScreenWidth),
      units::tileToPixel(Game::kScreenHeight),
      0);
   renderer_ = SDL_CreateRenderer(window_, -1, 0);
   SDL_ShowCursor(SDL_DISABLE);
}

Graphics::~Graphics() {
   for (SpriteMap::iterator iter = sprite_sheets_.begin();
        iter != sprite_sheets_.end();
        ++iter) {
      SDL_DestroyTexture(iter->second);
   }
   SDL_DestroyRenderer(renderer_);
   SDL_DestroyWindow(window_);
}

Graphics::SurfaceID Graphics::loadImage(const std::string& file_name, bool black_is_transparent) {
   const std::string file_path = config::getGraphicsQuality() == config::ORIGINAL_QUALITY ?
      "../content/original_graphics/" + file_name + ".pbm" :
      "../content/" + file_name + ".bmp";
   // if we have not loaded in the spritesheet
   if (sprite_sheets_.count(file_path) == 0) {
      // load it in now
      SDL_Surface* image = SDL_LoadBMP(file_path.c_str());
      if (!image) {
#ifndef _WIN32
         fprintf(stderr, "\nCould not load image: %s\nRelative to:          %s\n\n",
               file_path.c_str(),
               boost::filesystem::initial_path().c_str());
#endif
         exit(EXIT_FAILURE);
      }
      if (black_is_transparent) {
         const Uint32 black_color = SDL_MapRGB(image->format, 0, 0, 0);
         SDL_SetColorKey(image, SDL_TRUE, black_color);
      }
      sprite_sheets_[file_path] = SDL_CreateTextureFromSurface(renderer_, image);
      SDL_FreeSurface(image);
   }
   return sprite_sheets_[file_path];
}

void Graphics::blitSurface(
      SurfaceID source,
      SDL_Rect* source_rectangle,
      SDL_Rect* destination_rectangle) {
   //SDL_BlitSurface(source, source_rectangle, screen_, destination_rectangle);
   if (source_rectangle) {
      destination_rectangle->w = source_rectangle->w;
      destination_rectangle->h = source_rectangle->h;
   } else {
      uint32_t format;
      int access, w, h;
      SDL_QueryTexture(source, &format, &access, &w, &h);
      destination_rectangle->w = w;
      destination_rectangle->h = h;
   }
   SDL_RenderCopy(renderer_, source, source_rectangle, destination_rectangle);
}

void Graphics::drawRect(const Rectangle& rectangle,
   unsigned char r,
   unsigned char g,
   unsigned char b) {
   SDL_Rect dest_rect = { 
      units::gameToPixel(rectangle.left()),
      units::gameToPixel(rectangle.top()),
      units::gameToPixel(rectangle.width()),
      units::gameToPixel(rectangle.height())
   };
   SDL_SetRenderDrawColor(renderer_, r, g, b, 255);
   SDL_RenderFillRect(renderer_, &dest_rect);
}

void Graphics::drawRectOutline(const Rectangle& rectangle, units::Game thickness,
   unsigned char r,
   unsigned char g,
   unsigned char b) {
   drawRect(Rectangle(rectangle.left(), rectangle.top(), rectangle.width(), thickness), r, g, b);
   drawRect(Rectangle(rectangle.right() - thickness, rectangle.top(), thickness, rectangle.height()), r, g, b);
   drawRect(Rectangle(rectangle.left(), rectangle.bottom() - thickness, rectangle.width(), thickness), r, g, b);
   drawRect(Rectangle(rectangle.left(), rectangle.top(), thickness, rectangle.height()), r, g, b);
}
void Graphics::clear() {
   SDL_RenderClear(renderer_);
}

void Graphics::flip() {
   SDL_RenderPresent(renderer_);
}
