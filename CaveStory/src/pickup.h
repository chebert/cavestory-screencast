#ifndef PICKUP_H_
#define PICKUP_H_

#include "rectangle.h"

struct Graphics;
struct Map;

struct Pickup {
   enum PickupType {
      HEALTH,
      MISSILES,
      EXPERIENCE
   };

   virtual Rectangle collisionRectangle() const = 0;
   virtual void draw(Graphics& graphics) = 0;
   virtual bool update(units::MS elapsed_time, const Map& map) = 0;
   virtual int value() const = 0;
   virtual PickupType type() const = 0;
   virtual ~Pickup() = 0;
};
inline Pickup::~Pickup() {}

#endif // PICKUP_H_