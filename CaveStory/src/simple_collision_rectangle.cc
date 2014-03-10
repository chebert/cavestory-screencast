#include "simple_collision_rectangle.h"

const units::Game kExtraOffset = 0.001f;

Rectangle SimpleCollisionRectangle::boundingBox() const {
   return Rectangle(
      rectangle_.left() - kExtraOffset,
      rectangle_.top() - kExtraOffset,
      rectangle_.width() + 2*kExtraOffset,
      rectangle_.height() + 2*kExtraOffset);
}