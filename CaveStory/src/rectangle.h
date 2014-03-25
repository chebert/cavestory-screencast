#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "side_type.h"
#include "units.h"

struct Rectangle {
   Rectangle(units::Game x, units::Game y, units::Game width, units::Game height) :
      x_(x),
      y_(y),
      width_(width),
      height_(height) {}

   units::Game center_x() const { return x_ + width_ / 2; }
   units::Game center_y() const { return y_ + height_ / 2; }

   units::Game side(sides::SideType side) const {
      if (side == sides::LEFT_SIDE)
         return left();
      if (side == sides::RIGHT_SIDE)
         return right();
      if (side == sides::TOP_SIDE)
         return top();
      return bottom();
   }

   units::Game left() const { return x_; }
   units::Game right() const { return x_ + width_; }
   units::Game top() const { return y_; }
   units::Game bottom() const { return y_ + height_; }

   units::Game width() const { return width_; }
   units::Game height() const { return height_; }

   bool collidesWith(const Rectangle& other) const {
      return right() >= other.left() &&
             left() <= other.right() &&
             top () <= other.bottom() &&
             bottom() >= other.top();
   }
  private:
   const units::Game x_, y_, width_, height_;
};

#endif // RECTANGLE_H_
