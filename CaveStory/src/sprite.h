#ifndef SPRITE_H_
#define SPRITE_H_

#include <string>
#include <SDL/SDL.h>

struct Graphics;

struct Sprite {
   Sprite(Graphics& graphics,
          const std::string& file_path,
          int source_x, int source_y,
          int width, int height);

   virtual void update(int /*elapsed_time_ms*/) {}
   void draw(Graphics& graphics, int x, int y);

  protected:
   SDL_Rect source_rect_;

  private:
   SDL_Surface* sprite_sheet_;
};

#endif // SPRITE_H_
