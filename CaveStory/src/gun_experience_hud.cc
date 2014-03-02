#include "gun_experience_hud.h"

#include "number_sprite.h"

const units::Game kDrawY = 3 * units::kHalfTile;

const units::Game kLevelNumberDrawX = 3 * units::kHalfTile;

const units::Tile kLevelDrawX = 1;
const units::Tile kLevelSourceX = 5;
const units::Game kLevelSourceY = 160;
const units::Tile kLevelSourceWidth = 1;
const units::Game kLevelSourceHeight = units::kHalfTile;

const units::Game kExperienceBarDrawX = 5 * units::kHalfTile;
const units::Game kExperienceBarSourceX = 0;
const units::Game kExperienceBarSourceY = 9 * units::kHalfTile;
const units::Game kExperienceBarSourceWidth = 5 * units::kHalfTile;
const units::Game kExperienceBarSourceHeight = units::kHalfTile;

const std::string kSpriteName("TextBox");

GunExperienceHUD::GunExperienceHUD(Graphics& graphics) :
   experience_bar_sprite_(graphics, kSpriteName,
      units::gameToPixel(kExperienceBarSourceX),
      units::gameToPixel(kExperienceBarSourceY),
      units::gameToPixel(kExperienceBarSourceWidth),
      units::gameToPixel(kExperienceBarSourceHeight)),
   level_sprite_(graphics, kSpriteName,
      units::tileToPixel(kLevelSourceX),
      units::gameToPixel(kLevelSourceY),
      units::tileToPixel(kLevelSourceWidth),
      units::gameToPixel(kLevelSourceHeight)) {}

void GunExperienceHUD::draw(Graphics& graphics, units::GunLevel gun_level) {
   level_sprite_.draw(graphics, units::tileToGame(kLevelDrawX), kDrawY);
   NumberSprite::HUDNumber(graphics, gun_level, 2).draw(graphics, kLevelNumberDrawX, kDrawY);
   experience_bar_sprite_.draw(graphics, kExperienceBarDrawX, kDrawY);
}