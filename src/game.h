#ifndef GAME_H_
#define GAME_H_

struct SDL_Surface;

struct Game {
   Game();
   ~Game();

  private:
   void eventLoop();
   void update();
   void draw();

   SDL_Surface* screen_;
};

#endif // GAME_H_
