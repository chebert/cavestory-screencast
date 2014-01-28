#include "polar_star.h"

#include "sprite.h"

namespace {
const int kPolarStarIndex = 2; // 0-based indexing
const units::Game kGunWidth = 3 * units::kHalfTile;
const units::Game kGunHeight = 2 * units::kHalfTile;

const units::Tile kHorizontalOffset = 0;
const units::Tile kUpOffset = 2;
const units::Tile kDownOffset = 4;

const units::Tile kLeftOffset = 0;
const units::Tile kRightOffset = 1;

const std::string kSpritePath("../content/Arms.bmp");
}

PolarStar::PolarStar(Graphics& graphics) {
   initializeSprites(graphics);
}

void PolarStar::draw(
      Graphics& graphics,
      HorizontalFacing horizontal_facing, VerticalFacing vertical_facing,
      units::Game x, units::Game y) {
   if (horizontal_facing == LEFT)
      x -= units::kHalfTile;

   if (vertical_facing == UP)
      y -= units::kHalfTile / 2;
   if (vertical_facing == DOWN)
      y += units::kHalfTile / 2;
   sprite_map_[SpriteState(horizontal_facing, vertical_facing)]->draw(graphics, x, y);
}

void PolarStar::initializeSprites(Graphics& graphics) {
   for (int h_facing = FIRST_HORIZONTAL_FACING;
            h_facing < LAST_HORIZONTAL_FACING;
            ++h_facing) {
      for (int v_facing = FIRST_VERTICAL_FACING;
               v_facing < LAST_VERTICAL_FACING;
               ++v_facing) {
         initializeSprite(graphics, SpriteState((HorizontalFacing)h_facing,
                                                (VerticalFacing)v_facing));
      }
   }
}

void PolarStar::initializeSprite(Graphics& graphics, const SpriteState& sprite_state) {
   units::Tile tile_y = sprite_state.horizontal_facing == LEFT ? kLeftOffset : kRightOffset;
   switch (sprite_state.vertical_facing) {
      case HORIZONTAL:
         tile_y += kHorizontalOffset;
         break;
      case UP:
         tile_y += kUpOffset;
         break;
      case DOWN:
         tile_y += kDownOffset;
         break;
      case LAST_VERTICAL_FACING: break;
   }
   sprite_map_[sprite_state] = boost::shared_ptr<Sprite>(new Sprite(graphics, kSpritePath,
      units::gameToPixel(kPolarStarIndex * kGunWidth), units::tileToPixel(tile_y),
      units::gameToPixel(kGunWidth), units::gameToPixel(kGunHeight)));
}