#include "player.h"

#include "collision_rectangle.h"
#include "sprite.h"
#include "game.h"
#include "animated_sprite.h"
#include "graphics.h"
#include "map.h"
#include "rectangle.h"
#include "number_sprite.h"
#include "particle_system.h"
#include "head_bump_particle.h"

#include <cmath>

namespace {
// Walk Motion
const units::Acceleration kWalkingAcceleration = 0.00083007812f;
const units::Velocity kMaxSpeedX = 0.15859375f;
const units::Acceleration kFriction = 0.00049804687f;

// Fall Motion
const units::Acceleration kGravity = 0.00078125f;
const units::Velocity kMaxSpeedY = 0.2998046875f;

// Jump Motion
const units::Velocity kJumpSpeed = 0.25f;
const units::Velocity kShortJumpSpeed = kJumpSpeed / 1.5f;
const units::Acceleration kAirAcceleration = 0.0003125f;
const units::Acceleration kJumpGravity = 0.0003125f;

// Sprites
const std::string kSpriteFilePath("MyChar");

// Sprite Frames
const units::Frame kCharacterFrame = 0;

const units::Frame kWalkFrame = 0;
const units::Frame kStandFrame = 0;
const units::Frame kJumpFrame = 1;
const units::Frame kFallFrame = 2;
const units::Frame kUpFrameOffset = 3;
const units::Frame kDownFrame = 6;
const units::Frame kBackFrame = 7;

// Collision Rectangle
const units::Game kCollisionYTop = 2;
const units::Game kCollisionYHeight = 30;
const units::Game kCollisionTopWidth = 18;
const units::Game kCollisionBottomWidth = 10;
const units::Game kCollisionTopLeft = (units::tileToGame(1) - kCollisionTopWidth) / 2;
const units::Game kCollisionBottomLeft = (units::tileToGame(1) - kCollisionBottomWidth) / 2;
const CollisionRectangle kCollisionRectangle(
   Rectangle(kCollisionTopLeft, kCollisionYTop, kCollisionTopWidth, kCollisionYHeight / 2),
   Rectangle(kCollisionBottomLeft, kCollisionYTop + kCollisionYHeight / 2,
             kCollisionBottomWidth, kCollisionYHeight / 2),
   Rectangle(6, 10, 10, 12),
   Rectangle(16, 10, 10, 12));

const units::MS kInvincibleFlashTime = 50;
const units::MS kInvincibleTime = 3000;
}

Player::Player(Graphics& graphics, ParticleTools& particle_tools, units::Game x, units::Game y) :
   particle_tools_(particle_tools),
   kinematics_x_(x, 0.0f),
   kinematics_y_(y, 0.0f),
   acceleration_x_(0),
   horizontal_facing_(LEFT),
   intended_vertical_facing_(HORIZONTAL),
   on_ground_(false),
   jump_active_(false),
   interacting_(false),
   health_(graphics),
   invincible_timer_(kInvincibleTime),
   damage_text_(new DamageText()),
   gun_experience_hud_(graphics),
   polar_star_(graphics)
{
   initializeSprites(graphics);
}

void Player::update(units::MS elapsed_time_ms, const Map& map) {
   health_.update();

   walking_animation_.update();

   polar_star_.updateProjectiles(elapsed_time_ms, map, particle_tools_);

   updateX(elapsed_time_ms, map);
   updateY(elapsed_time_ms, map);
}

void Player::draw(Graphics& graphics) {
   if (spriteIsVisible()) {
      polar_star_.draw(graphics, horizontal_facing_, vertical_facing(), gun_up(), kinematics_x_.position, kinematics_y_.position);
      sprites_[getSpriteState()]->draw(graphics, kinematics_x_.position, kinematics_y_.position);
   }
}

void Player::drawHUD(Graphics& graphics) {
   if (spriteIsVisible()) {
      health_.draw(graphics);
      polar_star_.drawHUD(graphics, gun_experience_hud_);
   }
}

void Player::startMovingLeft() {
   if (on_ground() && acceleration_x_ == 0) walking_animation_.reset();
   acceleration_x_ = -1;
   horizontal_facing_ = LEFT;
   interacting_ = false;
}

void Player::startMovingRight() {
   if (on_ground() && acceleration_x_ == 0) walking_animation_.reset();
   acceleration_x_ = 1;
   horizontal_facing_ = RIGHT;
   interacting_ = false;
}

void Player::stopMoving() {
   acceleration_x_ = 0;
}

void Player::lookUp() {
   interacting_ = false;
   intended_vertical_facing_ = UP;
   interacting_ = false;
}

void Player::lookDown() {
   if (intended_vertical_facing_ == DOWN) return;
   intended_vertical_facing_ = DOWN;
   interacting_ = on_ground();
}

void Player::lookHorizontal() {
   intended_vertical_facing_ = HORIZONTAL;
}

void Player::startFire() {
   polar_star_.startFire(kinematics_x_.position, kinematics_y_.position, horizontal_facing_, vertical_facing(), gun_up(), particle_tools_);
}

void Player::stopFire() {
   polar_star_.stopFire();
}

void Player::startJump() {
   interacting_ = false;
   jump_active_ = true;
   if (on_ground()) {
      // give ourselves an initial velocity up
      kinematics_y_.velocity = -kJumpSpeed;
   }
   interacting_ = false;
}

void Player::stopJump() {
   jump_active_ = false;
}

void Player::takeDamage(units::HP damage) {
   if (invincible_timer_.active()) return;

   health_.takeDamage(damage);
   damage_text_->setDamage(damage);

   kinematics_y_.velocity = std::min(kinematics_y_.velocity, -kShortJumpSpeed);
   invincible_timer_.reset();
}

Rectangle Player::damageRectangle() const {
   return Rectangle(kinematics_x_.position + kCollisionRectangle.boundingBox().left(),
                    kinematics_y_.position + kCollisionRectangle.boundingBox().top(),
                    kCollisionRectangle.boundingBox().width(),
                    kCollisionRectangle.boundingBox().height());
}

void Player::initializeSprites(Graphics& graphics) {
   ENUM_FOREACH(motion, MOTION_TYPE) {
      ENUM_FOREACH(h_facing, HORIZONTAL_FACING) {
         ENUM_FOREACH(v_facing, VERTICAL_FACING) {
            ENUM_FOREACH(stride_type, STRIDE_TYPE) {
               initializeSprite(graphics, boost::make_tuple((MotionType)motion,
                  (HorizontalFacing)h_facing,
                  (VerticalFacing)v_facing,
                  (StrideType)stride_type));
            }
         }
      }
   }
}

void Player::initializeSprite(Graphics& graphics,
                              const SpriteState& sprite_state) {
   units::Tile tile_y = sprite_state.horizontal_facing() == LEFT ?
      kCharacterFrame :
      1 + kCharacterFrame;

   units::Tile tile_x;
   switch (sprite_state.motion_type()) {
      case WALKING:
         tile_x = kWalkFrame;
         break;
      case STANDING:
         tile_x = kStandFrame;
         break;
      case INTERACTING:
         tile_x = kBackFrame;
         break;
      case JUMPING:
         tile_x = kJumpFrame;
         break;
      case FALLING:
         tile_x = kFallFrame;
         break;
      case LAST_MOTION_TYPE:
         break;
   }
   switch (sprite_state.vertical_facing()) {
      case HORIZONTAL: break;
      case UP:
         tile_x += kUpFrameOffset;
         break;
      case DOWN:
         tile_x = kDownFrame;
         break;
      default: break;
   }

   if (sprite_state.motion_type() == WALKING) {
      switch (sprite_state.stride_type()) {
         case STRIDE_MIDDLE:
            break;
         case STRIDE_LEFT:
            tile_x += 1;
            break;
         case STRIDE_RIGHT:
            tile_x += 2;
            break;
         default: break;
      }
      sprites_[sprite_state] =
         boost::shared_ptr<Sprite>(new Sprite(
            graphics,
            kSpriteFilePath,
            units::tileToPixel(tile_x), units::tileToPixel(tile_y),
            units::tileToPixel(1), units::tileToPixel(1)));
   } else {
      sprites_[sprite_state] =
         boost::shared_ptr<Sprite>(new Sprite(
            graphics,
            kSpriteFilePath,
            units::tileToPixel(tile_x), units::tileToPixel(tile_y),
            units::tileToPixel(1), units::tileToPixel(1)));
   }
}

Player::MotionType Player::motionType() const {
   MotionType motion;
   if (interacting_) {
      motion = INTERACTING;
   } else if (on_ground()) {
      motion = acceleration_x_ == 0 ? STANDING : WALKING;
   } else {
      motion = kinematics_y_.velocity < 0.0f ? JUMPING : FALLING;
   }
   return motion;
}

Player::SpriteState Player::getSpriteState() {
   return boost::make_tuple(
         motionType(),
         horizontal_facing_,
         vertical_facing(),
         walking_animation_.stride()
         );
}

void Player::updateX(units::MS elapsed_time_ms, const Map& map) {
   // Update Velocity
   units::Acceleration acceleration_x = 0.0f;
   if (acceleration_x_ < 0) acceleration_x = on_ground() ? -kWalkingAcceleration : -kAirAcceleration;
   else if (acceleration_x_ > 0) acceleration_x = on_ground() ? kWalkingAcceleration : kAirAcceleration;

   kinematics_x_.velocity += acceleration_x * elapsed_time_ms;

   if (acceleration_x_ < 0) {
      kinematics_x_.velocity = std::max(kinematics_x_.velocity, -kMaxSpeedX);
   }
   else if (acceleration_x_ > 0) {
      kinematics_x_.velocity = std::min(kinematics_x_.velocity, kMaxSpeedX);
   }
   else if (on_ground()) {
      kinematics_x_.velocity = kinematics_x_.velocity > 0.0f ?
         std::max(0.0f, kinematics_x_.velocity - kFriction * elapsed_time_ms) :
         std::min(0.0f, kinematics_x_.velocity + kFriction * elapsed_time_ms);
   }

   MapCollidable::updateX(kCollisionRectangle, kinematics_x_, kinematics_y_, elapsed_time_ms, map);
}

void Player::updateY(units::MS elapsed_time_ms, const Map& map) {
   // Update Velocity
   const units::Acceleration gravity = jump_active_ && kinematics_y_.velocity < 0.0f ? kJumpGravity : kGravity;
   kinematics_y_.velocity = std::min(kinematics_y_.velocity + gravity * elapsed_time_ms, kMaxSpeedY);

   MapCollidable::updateY(kCollisionRectangle, kinematics_x_, kinematics_y_, elapsed_time_ms, map);
}

void Player::onCollision(MapCollidable::SideType side, bool is_delta_direction) {
   switch (side) {
      case MapCollidable::TOP_SIDE:
         if (is_delta_direction)
            kinematics_y_.velocity = 0.0f;
         particle_tools_.front_system.addNewParticle(boost::shared_ptr<Particle>(
            new HeadBumpParticle(particle_tools_.graphics, center_x(), kinematics_y_.position + kCollisionRectangle.boundingBox().top())));
         break;
      case MapCollidable::BOTTOM_SIDE:
         on_ground_ = true;
         if (is_delta_direction)
            kinematics_y_.velocity = 0.0f;
         break;
      case MapCollidable::LEFT_SIDE:
         if (is_delta_direction)
            kinematics_x_.velocity = 0.0f;
         break;
      case MapCollidable::RIGHT_SIDE:
         if (is_delta_direction)
            kinematics_x_.velocity = 0.0f;
         break;
   }
}

void Player::onDelta(MapCollidable::SideType side) {
   switch (side) {
      case MapCollidable::TOP_SIDE:
         on_ground_ = false;
         break;
      case MapCollidable::BOTTOM_SIDE:
         on_ground_ = false;
         break;
      case MapCollidable::LEFT_SIDE:
         break;
      case MapCollidable::RIGHT_SIDE:
         break;
   }
}

bool Player::spriteIsVisible() const {
   // % 2 == 0: 1 part invisible 1 part visible
   // % 3 == 0: 1 part invisible 2 part visible
   return !(invincible_timer_.active() &&
            invincible_timer_.current_time() / kInvincibleFlashTime % 2 == 0);
}
