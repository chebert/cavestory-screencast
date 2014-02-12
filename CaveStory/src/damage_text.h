#ifndef DAMAGE_TEXT_H_
#define DAMAGE_TEXT_H_

#include "units.h"
#include "timer.h"

struct Graphics;

struct DamageText {
   DamageText();

   void setDamage(units::HP damage);
   bool update(units::MS elapsed_time);
   void setPosition(units::Game center_x, units::Game center_y);
   void draw(Graphics& graphics);
  private:
   bool should_rise_;
   units::Game center_x_, center_y_;
   units::HP damage_;
   units::Game offset_y_;
   Timer timer_;
};

#endif // DAMAGE_TEXT_H_