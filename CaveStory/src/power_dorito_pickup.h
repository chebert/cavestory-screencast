#ifndef POWER_DORITO_PICKUP_H_
#define POWER_DORITO_PICKUP_H_

#include "animated_sprite.h"
#include "kinematics.h"
#include "map_collidable.h"
#include "pickup.h"

struct PowerDoritoPickup : Pickup, 
                           private MapCollidable {
   enum SizeType {
      SMALL = 0,
      MEDIUM = 1,
      LARGE = 2
   };

   PowerDoritoPickup(Graphics& graphics, units::Game center_x, units::Game center_y, SizeType size);
   
   Rectangle collisionRectangle() const;
   void draw(Graphics& graphics);
   bool update(units::MS elapsed_time, const Map& map);
   int value() const;
   PickupType type() const { return EXPERIENCE; }

  private:
   void onCollision(sides::SideType side, bool is_delta_direction, const tiles::TileType& tile_type);
   void onDelta(sides::SideType) {}

   Kinematics kinematics_x_, kinematics_y_;
   AnimatedSprite sprite_;
   SizeType size_;
   Timer timer_;
};

#endif // POWER_DORITO_PICKUP_H_
