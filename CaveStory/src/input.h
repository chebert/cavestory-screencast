#ifndef INPUT_H_
#define INPUT_H_

#include <SDL/SDL.h>
#include <map>

struct Input {
   void beginNewFrame();

   void keyDownEvent(const SDL_Event& event);
   void keyUpEvent(const SDL_Event& event);

   bool wasKeyPressed(SDLKey key);
   bool wasKeyReleased(SDLKey key);
   bool isKeyHeld(SDLKey key);

  private:
   std::map<SDLKey, bool> held_keys_;
   std::map<SDLKey, bool> pressed_keys_;
   std::map<SDLKey, bool> released_keys_;
};

#endif // INPUT_H_
