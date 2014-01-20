#include "backdrop.h"

#include <SDL/SDL.h>

#include "game.h"

namespace {
const int kBackgroundSize = Game::kTileSize * 4; // pixels
}

FixedBackdrop::FixedBackdrop(const std::string& path, Graphics& graphics) {
   surface_id_ = graphics.loadImage(path);
}

void FixedBackdrop::draw(Graphics& graphics) const {
   for (int x = 0; x < Game::kScreenWidth; x += kBackgroundSize) {
      for (int y = 0; y < Game::kScreenHeight; y += kBackgroundSize) {
         SDL_Rect destination_rectangle;
         destination_rectangle.x = x;
         destination_rectangle.y = y;
         graphics.blitSurface(surface_id_, NULL, &destination_rectangle);
      }
   }
}