#include "player.h"

#include "sprite.h"
#include "game.h"
#include "animated_sprite.h"
#include "graphics.h"

#include <cmath>

namespace {
// Walk Motion
const float kSlowdownFactor = 0.8f;
const float kWalkingAcceleration = 0.0012f; // (pixels / ms) / ms
const float kMaxSpeedX = 0.325f; // pixels / ms

// Fall Motion
const float kGravity = 0.0012f; // (pixels / ms) / ms
const float kMaxSpeedY = 0.325f; // pixels / ms

// Jump Motion
const float kJumpSpeed = 0.325f; // pixels / ms
const int kJumpTime = 275; // ms

// Sprites
const std::string kSpriteFilePath("content/MyChar.bmp");

// Sprite Frames
const int kCharacterFrame = 0;

const int kWalkFrame = 0;
const int kStandFrame = 0;
const int kJumpFrame = 1;
const int kFallFrame = 2;
const int kUpFrameOffset = 3;
const int kDownFrame = 6;
const int kBackFrame = 7;

// Walk Animation
const int kNumWalkFrames = 3;
const int kWalkFps = 15;
}

bool operator<(const Player::SpriteState& a, const Player::SpriteState& b) {
   if (a.motion_type != b.motion_type) {
      return a.motion_type < b.motion_type;
   }
   if (a.horizontal_facing != b.horizontal_facing) {
      return a.horizontal_facing < b.horizontal_facing;
   }
   if (a.vertical_facing != b.vertical_facing) {
      return a.vertical_facing < b.vertical_facing;
   }
   return false;
}

Player::Player(Graphics& graphics, int x, int y) :
   x_(x),
   y_(y),
   velocity_x_(0.0f),
   velocity_y_(0.0f),
   acceleration_x_(0.0f),
   horizontal_facing_(LEFT),
   vertical_facing_(HORIZONTAL),
   on_ground_(false)
{
   initializeSprites(graphics);
}

void Player::update(int elapsed_time_ms) {
   sprites_[getSpriteState()]->update(elapsed_time_ms);
   jump_.update(elapsed_time_ms);

   x_ += round(velocity_x_ * elapsed_time_ms);
   velocity_x_ += acceleration_x_ * elapsed_time_ms;
   if (acceleration_x_ < 0.0f) {
      velocity_x_ = std::max(velocity_x_, -kMaxSpeedX);
   } else if (acceleration_x_ > 0.0f) {
      velocity_x_ = std::min(velocity_x_, kMaxSpeedX);
   } else if (on_ground()) {
      velocity_x_ *= kSlowdownFactor;
   }

   y_ += round(velocity_y_ * elapsed_time_ms);
   if (!jump_.active()) {
      velocity_y_ = std::min(velocity_y_ + kGravity * elapsed_time_ms,
                             kMaxSpeedY);
   }

   // TODO: remove this hack
   if (y_ >= 320) {
      y_ = 320;
      velocity_y_ = 0.0f;
   }
   on_ground_ = y_ == 320;
   // TODO: remove this hack
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

void Player::lookUp() {
   vertical_facing_ = UP;
}

void Player::lookDown() {
   vertical_facing_ = DOWN;
}

void Player::lookHorizontal() {
   vertical_facing_ = HORIZONTAL;
}

void Player::startJump() {
   if (on_ground()) {
      jump_.reset();
      // give ourselves an initial velocity up
      velocity_y_ = -kJumpSpeed;
   // else if we are mid jump
   } else if (velocity_y_ < 0.0f) {
      jump_.reactivate();
   }
}

void Player::stopJump() {
   jump_.deactivate();
}

void Player::initializeSprites(Graphics& graphics) {
   for (MotionType motion_type = FIRST_MOTION_TYPE;
        motion_type < LAST_MOTION_TYPE;
        ++motion_type) {
      for (HorizontalFacing horizontal_facing = FIRST_HORIZONTAL_FACING;
           horizontal_facing < LAST_HORIZONTAL_FACING;
           ++horizontal_facing) {
         for (VerticalFacing vertical_facing = FIRST_VERTICAL_FACING;
              vertical_facing < LAST_VERTICAL_FACING;
              ++vertical_facing) {
            initializeSprite(graphics, SpriteState(motion_type,
                                                   horizontal_facing,
                                                   vertical_facing));
         }
      }
   }
}

void Player::initializeSprite(Graphics& graphics,
                              const SpriteState& sprite_state) {
   int source_y = sprite_state.horizontal_facing == LEFT ?
      kCharacterFrame*Game::kTileSize :
      (1 + kCharacterFrame)*Game::kTileSize;

   int source_x;
   switch (sprite_state.motion_type) {
      case WALKING:
         source_x = kWalkFrame  * Game::kTileSize;
         break;
      case STANDING:
         source_x = kStandFrame * Game::kTileSize;
         break;
      case JUMPING:
         source_x = kJumpFrame  * Game::kTileSize;
         break;
      case FALLING:
         source_x = kFallFrame * Game::kTileSize;
         break;
      case LAST_MOTION_TYPE:
         break;
   }
   source_x = sprite_state.vertical_facing == UP ?
      source_x + kUpFrameOffset*Game::kTileSize :
      source_x;

   if (sprite_state.motion_type == WALKING) {
      sprites_[sprite_state] =
         boost::shared_ptr<Sprite>(new AnimatedSprite(
            graphics,
            kSpriteFilePath,
            source_x, source_y,
            Game::kTileSize, Game::kTileSize,
            kWalkFps, kNumWalkFrames));
   } else {
      if (sprite_state.vertical_facing == DOWN) {
         source_x = sprite_state.motion_type == STANDING ?
            kBackFrame * Game::kTileSize :
            kDownFrame * Game::kTileSize;
      }
      sprites_[sprite_state] =
         boost::shared_ptr<Sprite>(new Sprite(
            graphics,
            kSpriteFilePath,
            source_x, source_y,
            Game::kTileSize, Game::kTileSize));
   }
}

Player::SpriteState Player::getSpriteState() {
   MotionType motion;
   if (on_ground()) {
      motion = acceleration_x_ == 0.0f ? STANDING : WALKING;
   } else {
      motion = velocity_y_ < 0.0f ? JUMPING : FALLING;
   }
   return SpriteState(
         motion,
         horizontal_facing_,
         vertical_facing_
         );
}

void Player::Jump::reset() {
   time_remaining_ms_ = kJumpTime;
   reactivate();
}

void Player::Jump::update(int elapsed_time_ms) {
   if (active_) {
      time_remaining_ms_ -= elapsed_time_ms;
      if (time_remaining_ms_ <= 0) {
         active_ = false;
      }
   }
}
