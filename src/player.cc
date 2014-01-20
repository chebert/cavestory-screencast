#include "player.h"

#include "sprite.h"
#include "game.h"
#include "animated_sprite.h"
#include "graphics.h"

#include <cmath>

namespace {
const float kSlowdownFactor = 0.8f;
const float kWalkingAcceleration = 0.0012f; // (pixels / ms) / ms
const float kMaxSpeedX = 0.325f; // pixels / ms
}

bool operator<(const Player::SpriteState& a, const Player::SpriteState& b) {
   if (a.motion_type != b.motion_type) {
      return a.motion_type < b.motion_type;
   }
   if (a.horizontal_facing != b.horizontal_facing) {
      return a.horizontal_facing < b.horizontal_facing;
   }
   return false;
}

Player::Player(Graphics& graphics, int x, int y) :
   x_(x),
   y_(y),
   velocity_x_(0.0f),
   acceleration_x_(0.0f),
   horizontal_facing_(LEFT)
{
   initializeSprites(graphics);
}

void Player::update(int elapsed_time_ms) {
   sprites_[getSpriteState()]->update(elapsed_time_ms);

   x_ += round(velocity_x_ * elapsed_time_ms);
   velocity_x_ += acceleration_x_ * elapsed_time_ms;
   if (acceleration_x_ < 0.0f) {
      velocity_x_ = std::max(velocity_x_, -kMaxSpeedX);
   } else if (acceleration_x_ > 0.0f) {
      velocity_x_ = std::min(velocity_x_, kMaxSpeedX);
   } else {
      velocity_x_ *= kSlowdownFactor;
   }
}

void Player::draw(Graphics& graphics) {
   sprites_[getSpriteState()]->draw(graphics, x_, y_);
}

void Player::startMovingLeft() {
   acceleration_x_ = -kWalkingAcceleration;
   horizontal_facing_ = LEFT;
}

void Player::startMovingRight() {
   acceleration_x_ = kWalkingAcceleration;
   horizontal_facing_ = RIGHT;
}

void Player::stopMoving() {
   acceleration_x_ = 0.0f;
}

void Player::initializeSprites(Graphics& graphics) {
   sprites_[SpriteState(STANDING, LEFT)] =
      boost::shared_ptr<Sprite>(new Sprite(
         graphics,
         "content/MyChar.bmp",
         0, 0, Game::kTileSize, Game::kTileSize));
   sprites_[SpriteState(WALKING, LEFT)] =
      boost::shared_ptr<Sprite>(new AnimatedSprite(
         graphics,
         "content/MyChar.bmp",
         0, 0, Game::kTileSize, Game::kTileSize,
         15, 3));

   sprites_[SpriteState(STANDING, RIGHT)] =
      boost::shared_ptr<Sprite>(new Sprite(
         graphics,
         "content/MyChar.bmp",
         0, Game::kTileSize, Game::kTileSize, Game::kTileSize));
   sprites_[SpriteState(WALKING, RIGHT)] =
      boost::shared_ptr<Sprite>(new AnimatedSprite(
         graphics,
         "content/MyChar.bmp",
         0, Game::kTileSize, Game::kTileSize, Game::kTileSize,
         15, 3));
}

Player::SpriteState Player::getSpriteState() {
   return SpriteState(
         acceleration_x_ == 0.0f ? STANDING : WALKING,
         horizontal_facing_
         );
}
