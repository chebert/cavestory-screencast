#ifndef FLOATING_NUMBER_H_
#define FLOATING_NUMBER_H_

#include "units.h"
#include "timer.h"

struct Graphics;

struct FloatingNumber {
   enum NumberType {
      DAMAGE,
      EXPERIENCE
   };
   FloatingNumber(NumberType type);

   void addValue(int value);
   bool update(units::MS elapsed_time);
   void setPosition(units::Game center_x, units::Game center_y);
   void draw(Graphics& graphics);
  private:
   bool should_rise_;
   units::Game center_x_, center_y_;
   int value_;
   units::Game offset_y_;
   Timer timer_;
   const NumberType type_;
};

#endif // FLOATING_NUMBER_H_
