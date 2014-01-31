#include "player.h"

// Walk Animation
const units::Frame kNumWalkFrames = 3;
const units::FPS kWalkFps = 15;

Player::WalkingAnimation::WalkingAnimation() :
      frame_timer_(1000/kWalkFps),
      current_frame_(0),
      forward_(true) {}

void Player::WalkingAnimation::reset() {
   forward_ = true;
   current_frame_ = 0;
   frame_timer_.reset();
}

void Player::WalkingAnimation::update() {
   if (frame_timer_.expired()) {
      frame_timer_.reset();

      if (forward_) {
         ++current_frame_;
         forward_ = current_frame_ != kNumWalkFrames - 1;
      } else {
         --current_frame_;
         forward_ = current_frame_ == 0;
      }
   }
}

Player::StrideType Player::WalkingAnimation::stride() const {
   switch (current_frame_) {
      case 0:
         return STRIDE_LEFT;
      case 1:
         return STRIDE_MIDDLE;
      case 2:
         return STRIDE_RIGHT;
      default: return STRIDE_MIDDLE;
   }
}