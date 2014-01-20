#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <map>
#include <string>

struct SDL_Surface;
struct SDL_Rect;

struct Graphics {
   typedef SDL_Surface* SurfaceID;

   Graphics();
   ~Graphics();

   SurfaceID loadImage(const std::string& file_path);

   void blitSurface(
         SurfaceID source,
         SDL_Rect* source_rectangle,
         SDL_Rect* destination_rectangle);

   void clear();
   void flip();

  private:
   std::map<std::string, SDL_Surface*> sprite_sheets_;

   SDL_Surface* screen_;
};

#endif // GRAPHICS_H_
