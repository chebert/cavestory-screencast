#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "rectangle.h"
#include "units.h"

struct Projectile {
   virtual Rectangle collisionRectangle() const = 0;
   virtual units::HP contactDamage() const = 0;
   virtual void collideWithEnemy() = 0;
};

#endif // PROJECTILE_H_