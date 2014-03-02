#ifndef GUN_EXPERIENCE_HUD_H_
#define GUN_EXPERIENCE_HUD_H_

#include "sprite.h"
#include "timer.h"
#include "varying_width_sprite.h"

struct GunExperienceHUD {
   GunExperienceHUD(Graphics& graphics);

   void activateFlash() { flash_timer_.reset(); }
   void draw(Graphics& graphics,
      units::GunLevel gun_level,
      units::GunExperience current_experience,
      units::GunExperience level_experience);

  private:
   Sprite experience_bar_sprite_, level_sprite_, flash_sprite_, max_sprite_;
   VaryingWidthSprite fill_sprite_;
   Timer flash_timer_;
};

#endif // GUN_EXPERIENCE_HUD_H_