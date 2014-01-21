#include "game.h"

#include <SDL/SDL.h>

#include "graphics.h"
#include "map.h"
#include "player.h"
#include "input.h"

namespace {
const units::FPS kFps = 60;
const units::MS kMaxFrameTime = 5 * 1000 / 60;
}

//static
units::Tile Game::kScreenWidth = 20;
//static
units::Tile Game::kScreenHeight = 15;

Game::Game() {
   SDL_Init(SDL_INIT_EVERYTHING);
   eventLoop();
}

Game::~Game() {
   SDL_Quit();
}

void Game::eventLoop() {
   Graphics graphics;
   Input input;
   SDL_Event event;

   player_.reset(new Player(graphics, units::tileToGame(kScreenWidth / 2), units::tileToGame(kScreenHeight / 2)));
   map_.reset(Map::createTestMap(graphics));

   bool running = true;
   units::MS last_update_time = SDL_GetTicks();
   while (running) {
      const units::MS start_time_ms = SDL_GetTicks();
      input.beginNewFrame();
      while (SDL_PollEvent(&event)) {
         switch (event.type) {
            case SDL_KEYDOWN:
               input.keyDownEvent(event);
               break;
            case SDL_KEYUP:
               input.keyUpEvent(event);
               break;
            default:
               break;
         }
      }

      if (input.wasKeyPressed(SDLK_ESCAPE)) {
         running = false;
      }

      // Player Horizontal Movement
      // if both left and right are being pressed
      if (input.isKeyHeld(SDLK_LEFT) && input.isKeyHeld(SDLK_RIGHT)) {
         player_->stopMoving();
      } else if (input.isKeyHeld(SDLK_LEFT)) {
         player_->startMovingLeft();
      } else if (input.isKeyHeld(SDLK_RIGHT)) {
         player_->startMovingRight();
      } else {
         player_->stopMoving();
      }

      if (input.isKeyHeld(SDLK_UP) && input.isKeyHeld(SDLK_DOWN)) {
         player_->lookHorizontal();
      } else if (input.isKeyHeld(SDLK_UP)) {
         player_->lookUp();
      } else if (input.isKeyHeld(SDLK_DOWN)) {
         player_->lookDown();
      } else {
         player_->lookHorizontal();
      }

      // Player Jump
      if (input.wasKeyPressed(SDLK_z)) {
         player_->startJump();
      } else if (input.wasKeyReleased(SDLK_z)) {
         player_->stopJump();
      }

      const units::MS current_time_ms = SDL_GetTicks();
      const units::MS elapsed_time = current_time_ms - last_update_time;
      update(std::min(elapsed_time, kMaxFrameTime));
      last_update_time = current_time_ms;

      draw(graphics);
      const units::MS ms_per_frame = 1000/*ms*/ / kFps;
      const units::MS elapsed_time_ms = SDL_GetTicks() - start_time_ms;
      if (elapsed_time_ms < ms_per_frame) {
         SDL_Delay(ms_per_frame - elapsed_time_ms);
      }
   }
}

void Game::update(units::MS elapsed_time_ms) {
   player_->update(elapsed_time_ms, *map_);
   map_->update(elapsed_time_ms);
}

void Game::draw(Graphics& graphics) {
   graphics.clear();
   map_->drawBackground(graphics);
   player_->draw(graphics);
   map_->draw(graphics);
   graphics.flip();
}
