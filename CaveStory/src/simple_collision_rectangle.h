#ifndef SIMPLE_COLLISION_RECTANGLE_H_
#define SIMPLE_COLLISION_RECTANGLE_H_

#include <assert.h>

#include "collision_rectangle.h"

struct SimpleCollisionRectangle : CollisionRectangle {
   SimpleCollisionRectangle(const Rectangle& rectangle) :
      rectangle_(rectangle) {}

   Rectangle boundingBox() const; 

   Rectangle leftCollision(units::Game x, units::Game y, units::Game delta) const {
      assert(delta <= 0);
      return Rectangle(
         x + rectangle_.left() + delta,
         y + rectangle_.top(),
         rectangle_.width() - delta,
         rectangle_.height()); 
   }
   Rectangle rightCollision(units::Game x, units::Game y, units::Game delta) const {
      assert(delta >= 0);
      return Rectangle(
         x + rectangle_.left(),
         y + rectangle_.top(),
         rectangle_.width() + delta,
         rectangle_.height());
   }

   Rectangle topCollision(units::Game x, units::Game y, units::Game delta) const {
      assert(delta <= 0);
      return Rectangle(
         x + rectangle_.left(),
         y + rectangle_.top() + delta,
         rectangle_.width(),
         rectangle_.height() - delta); 
   }
   Rectangle bottomCollision(units::Game x, units::Game y, units::Game delta) const {
      assert(delta >= 0);
      return Rectangle(
         x + rectangle_.left(),
         y + rectangle_.top(),
         rectangle_.width(),
         rectangle_.height() + delta); 
   }

  private:
   Rectangle rectangle_;
};

#endif // SIMPLE_COLLISION_RECTANGLE_H_