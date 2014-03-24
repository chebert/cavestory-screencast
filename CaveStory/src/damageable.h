#ifndef DAMAGEABLE_H_
#define DAMAGEABLE_H_

#include <boost/shared_ptr.hpp>
#include "units.h"

struct FloatingNumber;

struct Damageable {
   virtual units::Game center_x() const = 0;
   virtual units::Game center_y() const = 0;
   virtual boost::shared_ptr<FloatingNumber> get_damage_text() = 0;

   virtual ~Damageable() = 0;
};

inline Damageable::~Damageable() {}

#endif // DAMAGEABLE_H_
