#ifndef GUN_EXPERIENCE_HUD_H_
#define GUN_EXPERIENCE_HUD_H_

#include "sprite.h"

struct GunExperienceHUD {
   GunExperienceHUD(Graphics& graphics);

   void draw(Graphics& graphics, units::GunLevel gun_level);

  private:
   Sprite experience_bar_sprite_, level_sprite_;
};

#endif // GUN_EXPERIENCE_HUD_H_