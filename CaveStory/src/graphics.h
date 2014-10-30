#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <map>
#include <string>

#include "units.h"

struct Rectangle;
struct SDL_Surface;
struct SDL_Rect;

struct Graphics {
   typedef SDL_Surface* SurfaceID;

   Graphics();
   ~Graphics();

   SurfaceID loadImage(const std::string& file_name, bool black_is_transparent=false);

   void blitSurface(
         SurfaceID source,
         SDL_Rect* source_rectangle,
         SDL_Rect* destination_rectangle);

   void drawRect(const Rectangle& rectangle,
      unsigned char r,
      unsigned char g,
      unsigned char b);
   void drawRectOutline(const Rectangle& rectangle, units::Game thickness,
      unsigned char r,
      unsigned char g,
      unsigned char b);

   void clear();
   void flip();

  private:
   typedef std::map<std::string, SDL_Surface*> SpriteMap;
   SpriteMap sprite_sheets_;

   SDL_Surface* screen_;
};

#endif // GRAPHICS_H_
