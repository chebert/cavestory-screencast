<<<<<<< HEAD:CaveStory/src/main.cc
#include "SDL/SDL.h"
=======
#include <SDL/SDL.h> // sdl does some setup magic for osx, so it must be defined here
>>>>>>> 41bfe0a... Fixes: OSX crash:src/main.cc
#include "game.h"

int main(int, char**) {
   Game game;

   return 0;
}
