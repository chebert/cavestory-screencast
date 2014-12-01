#ifndef INPUT_H_
#define INPUT_H_

#include <SDL.h>
#include <map>

struct Input {
   void beginNewFrame();

   void keyDownEvent(const SDL_Event& event);
   void keyUpEvent(const SDL_Event& event);

   bool wasKeyPressed(SDL_Scancode key);
   bool wasKeyReleased(SDL_Scancode key);
   bool isKeyHeld(SDL_Scancode key);

  private:
   std::map<SDL_Scancode, bool> held_keys_;
   std::map<SDL_Scancode, bool> pressed_keys_;
   std::map<SDL_Scancode, bool> released_keys_;
};

#endif // INPUT_H_
