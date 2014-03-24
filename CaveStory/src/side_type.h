#ifndef SIDE_TYPE_H_
#define SIDE_TYPE_H_

#include "sprite_state.h"

namespace sides {
   enum SideType {
      TOP_SIDE,
      BOTTOM_SIDE,
      LEFT_SIDE,
      RIGHT_SIDE,
   };

   inline SideType opposite_side(SideType side) {
      if (side == TOP_SIDE)
         return BOTTOM_SIDE;
      if (side == BOTTOM_SIDE)
         return TOP_SIDE;
      if (side == LEFT_SIDE)
         return RIGHT_SIDE;
      return LEFT_SIDE;
   }

   inline bool vertical(SideType side) {
      return side == TOP_SIDE || side == BOTTOM_SIDE;
   }

   inline bool horizontal(SideType side) {
      return !vertical(side);
   }

   inline SideType from_facing(HorizontalFacing h_facing, VerticalFacing v_facing) {
      if (v_facing == UP)
         return TOP_SIDE;
      if (v_facing == DOWN)
         return BOTTOM_SIDE;
      if (h_facing == LEFT)
         return LEFT_SIDE;
      return RIGHT_SIDE;
   }
}

#endif // SIDE_TYPE_H_
