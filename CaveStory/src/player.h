#ifndef PLAYER_H_
#define PLAYER_H_

#include "kinematics.h"
#include "map_collidable.h"
#include "sprite.h"
#include "sprite_state.h"
#include "varying_width_sprite.h"
#include "rectangle.h"
#include "number_sprite.h"
#include "units.h"
#include "timer.h"
#include "floating_number.h"
#include "damageable.h"
#include "polar_star.h"
#include "gun_experience_hud.h"

struct Graphics;
struct Projectile;
struct Map;
struct ParticleTools;
struct Pickup;

struct Player : public Damageable,
                private MapCollidable {
   Player(Graphics& graphics, ParticleTools& particle_tools, units::Game x, units::Game y);

   void update(units::MS elapsed_time_ms, const Map& map);
   void draw(Graphics& graphics);
   void drawHUD(Graphics& graphics);

   void startMovingLeft();
   void startMovingRight();
   void stopMoving();

   void lookUp();
   void lookDown();
   void lookHorizontal();

   void startFire();
   void stopFire();

   void startJump();
   void stopJump();

   void takeDamage(units::HP damage);
   void collectPickup(const Pickup& pickup);

   Rectangle damageRectangle() const; 
   units::Game center_x() const { return kinematics_x_.position + units::kHalfTile; }
   units::Game center_y() const { return kinematics_y_.position + units::kHalfTile; }
   boost::shared_ptr<FloatingNumber> get_damage_text() { return damage_text_; }

   std::vector<boost::shared_ptr<Projectile> > getProjectiles()
      { return polar_star_.getProjectiles(); }

  private:
   enum MotionType {
      FIRST_MOTION_TYPE,
      STANDING = FIRST_MOTION_TYPE,
      INTERACTING,
      WALKING,
      JUMPING,
      FALLING,
      LAST_MOTION_TYPE
   };
   enum StrideType {
      FIRST_STRIDE_TYPE,
      STRIDE_MIDDLE = FIRST_STRIDE_TYPE,
      STRIDE_LEFT,
      STRIDE_RIGHT,
      LAST_STRIDE_TYPE
   };

   typedef boost::tuple<MotionType, HorizontalFacing, VerticalFacing, StrideType> SpriteTuple;
   struct SpriteState : public SpriteTuple {
      SpriteState(const SpriteTuple& tuple) : SpriteTuple(tuple) {}

      MotionType motion_type() const { return get<0>(); }
      HorizontalFacing horizontal_facing() const { return get<1>(); }
      VerticalFacing vertical_facing() const { return get<2>(); }
      StrideType stride_type() const { return get<3>(); }
   };

   struct WalkingAnimation {
      WalkingAnimation();

      void update();
      void reset();

      StrideType stride() const;
     private:
      Timer frame_timer_;
      units::Frame current_frame_;
      bool forward_;
   };

   struct Health {
      Health(Graphics& graphics);

      void update();
      void draw(Graphics& graphics);

      // returns true if we have died.
      bool takeDamage(units::HP damage);
      void addHealth(units::HP health);

     private:
      void resetFillSprites();

      units::HP damage_;
      Timer damage_timer_;

      units::HP max_health_;
      units::HP current_health_;
      Sprite health_bar_sprite_;
      VaryingWidthSprite health_fill_sprite_;
      VaryingWidthSprite damage_fill_sprite_;
   };

   void initializeSprites(Graphics& graphics);
   void initializeSprite(Graphics& graphics, const SpriteState& sprite_state);
   SpriteState getSpriteState();

   void updateX(units::MS elapsed_time_ms, const Map& map);
   void updateY(units::MS elapsed_time_ms, const Map& map);

   void onCollision(sides::SideType side, bool is_delta_direction);
   void onDelta(sides::SideType side);

   bool spriteIsVisible() const;

   MotionType motionType() const;
   bool on_ground() const { return on_ground_; }
   bool gun_up() const
      { return motionType() == WALKING && walking_animation_.stride() != STRIDE_MIDDLE; }
   VerticalFacing vertical_facing() const {
      return on_ground() && intended_vertical_facing_ == DOWN ?
               HORIZONTAL : intended_vertical_facing_;
   }

   ParticleTools& particle_tools_;
   Kinematics kinematics_x_, kinematics_y_;
   int acceleration_x_;
   HorizontalFacing horizontal_facing_;
   VerticalFacing intended_vertical_facing_;
   bool on_ground_;
   bool jump_active_;
   bool interacting_;

   Health health_;
   Timer invincible_timer_;
   boost::shared_ptr<FloatingNumber> damage_text_;
   FloatingNumber experience_text_;

   WalkingAnimation walking_animation_;

   GunExperienceHUD gun_experience_hud_;
   PolarStar polar_star_;

   std::map<SpriteState, boost::shared_ptr<Sprite> > sprites_;
};

#endif // PLAYER_H_
