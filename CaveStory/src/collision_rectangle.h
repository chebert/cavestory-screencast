#ifndef COLLISION_RECTANGLE_H_
#define COLLISION_RECTANGLE_H_

#include <assert.h>

#include "rectangle.h"

struct CollisionRectangle {
   CollisionRectangle(
         const Rectangle& top,
         const Rectangle& bottom,
         const Rectangle& left,
         const Rectangle& right) :
      top_(top), bottom_(bottom),
      left_(left), right_(right) {}

   Rectangle boundingBox() const {
      return Rectangle(
         left_.left(),
         top_.top(),
         left_.width() + right_.width(),
         top_.height() + bottom_.height());
   }
   Rectangle leftCollision(units::Game x, units::Game y, units::Game delta) const {
      assert(delta <= 0);
      return Rectangle(
         x + left_.left() + delta,
         y + left_.top(),
         left_.width() - delta,
         left_.height()); 
   }
   Rectangle rightCollision(units::Game x, units::Game y, units::Game delta) const {
      assert(delta >= 0);
      return Rectangle(
         x + right_.left(),
         y + right_.top(),
         right_.width() + delta,
         right_.height());
   }

   Rectangle topCollision(units::Game x, units::Game y, units::Game delta) const {
      assert(delta <= 0);
      return Rectangle(
         x + top_.left(),
         y + top_.top() + delta,
         top_.width(),
         top_.height() - delta); 
   }
   Rectangle bottomCollision(units::Game x, units::Game y, units::Game delta) const {
      assert(delta >= 0);
      return Rectangle(
         x + bottom_.left(),
         y + bottom_.top(),
         bottom_.width(),
         bottom_.height() + delta); 
   }

  private:
   Rectangle top_, bottom_, left_, right_;
};

#endif // COLLISION_RECTANGLE_H_