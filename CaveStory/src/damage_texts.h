#ifndef DAMAGE_TEXTS_H_
#define DAMAGE_TEXTS_H_

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <map>

#include "units.h"

struct FloatingNumber;
struct Damageable;
struct Graphics;

struct DamageTexts {
   void addDamageable(boost::shared_ptr<Damageable> damageable);
   void update(units::MS elapsed_time);
   void draw(Graphics& graphics);

  private:
   typedef std::map<boost::shared_ptr<FloatingNumber>, boost::weak_ptr<Damageable> > DamageTextMap;

   DamageTextMap damage_text_map_;
};

#endif // DAMAGE_TEXTS_H_
