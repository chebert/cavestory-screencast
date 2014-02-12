#include "backdrop.h"

#include <SDL/SDL.h>

#include "game.h"

namespace {
const units::Tile kBackgroundSize = 4;
}

FixedBackdrop::FixedBackdrop(const std::string& path, Graphics& graphics) {
   surface_id_ = graphics.loadImage(path);
}

void FixedBackdrop::draw(Graphics& graphics) const {
   for (units::Tile x = 0; x < Game::kScreenWidth; x += kBackgroundSize) {
      for (units::Tile y = 0; y < Game::kScreenHeight; y += kBackgroundSize) {
         SDL_Rect destination_rectangle;
         destination_rectangle.x = units::tileToPixel(x);
         destination_rectangle.y = units::tileToPixel(y);
         graphics.blitSurface(surface_id_, NULL, &destination_rectangle);
      }
   }
}