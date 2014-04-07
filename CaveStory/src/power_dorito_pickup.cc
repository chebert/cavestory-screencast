#include "power_dorito_pickup.h"

#include <string>

#include "accelerators.h"
#include "simple_collision_rectangle.h"

const units::GunExperience kValues[] = { 1, 5, 20 };

const std::string kSpriteName("NpcSym");
const units::Tile kSourceX = 0;
const units::Tile kSourceYs[] = { 1, 2, 3 };
const units::Tile kSourceWidth = 1;
const units::Tile kSourceHeight = 1;
const units::FPS kFps = 14;
const units::Frame kNumFrames = 6;

const units::MS kLifeTime = 8000;
const units::MS kFlashTime = 7000;
const units::MS kFlashPeriod = 50;

const units::Velocity kBounceSpeed = 0.225f;

const FrictionAccelerator kFriction(0.00002f);
const SimpleCollisionRectangle kCollisionRectangles[] = {
   SimpleCollisionRectangle(Rectangle(8, 8, 16, 16)),
   SimpleCollisionRectangle(Rectangle(4, 4, 24, 24)),
   SimpleCollisionRectangle(Rectangle(0, 0, 32, 32))
};

PowerDoritoPickup::PowerDoritoPickup(Graphics& graphics,
      units::Game center_x, units::Game center_y, SizeType size) :
   kinematics_x_(center_x - units::kHalfTile, (rand() % 11 - 5) * 0.025f),
   kinematics_y_(center_y - units::kHalfTile, (rand() % 11 - 5) * 0.025f),
   sprite_(graphics, kSpriteName,
      units::tileToPixel(kSourceX), units::tileToPixel(kSourceYs[size]),
      units::tileToPixel(kSourceWidth), units::tileToPixel(kSourceHeight),
      kFps, kNumFrames),
   size_(size),
   timer_(kLifeTime, true) {}


Rectangle PowerDoritoPickup::collisionRectangle() const {
   return Rectangle(
      kinematics_x_.position + kCollisionRectangles[size_].boundingBox().left(),
      kinematics_y_.position + kCollisionRectangles[size_].boundingBox().top(),
      kCollisionRectangles[size_].boundingBox().width(),
      kCollisionRectangles[size_].boundingBox().height()
   );
}

void PowerDoritoPickup::draw(Graphics& graphics) {
   if (timer_.current_time() < kFlashTime || timer_.current_time() / kFlashPeriod % 2 == 0) {
      sprite_.draw(graphics, kinematics_x_.position, kinematics_y_.position);
   }
}

bool PowerDoritoPickup::update(units::MS elapsed_time, const Map& map) {
   sprite_.update();

   MapCollidable::updateX(kCollisionRectangles[size_], kFriction,
      kinematics_x_, kinematics_y_, elapsed_time, map);
   MapCollidable::updateY(kCollisionRectangles[size_], ConstantAccelerator::kGravity,
      kinematics_x_, kinematics_y_, elapsed_time, map, boost::none);

   return timer_.active();
}

void PowerDoritoPickup::onCollision(sides::SideType side, bool /*is_delta_direction*/, const tiles::TileType& tile_type) {
   if (side == sides::TOP_SIDE) {
      kinematics_y_.velocity = 0.0f;
   } else if (side == sides::BOTTOM_SIDE) {
      kinematics_y_.velocity = -kBounceSpeed;
   } else {
      kinematics_x_.velocity = - kinematics_x_.velocity;
   }
}

int PowerDoritoPickup::value() const {
   return kValues[size_];
}
