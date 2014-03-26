#include "game.h"

#include <SDL/SDL.h>
#include <stdlib.h>
#include <time.h>

#include "graphics.h"
#include "map.h"
#include "player.h"
#include "input.h"
#include "first_cave_bat.h"
#include "timer.h"

#include "death_cloud_particle.h"
#include "power_dorito_pickup.h"
#include "flashing_pickup.h"

namespace {
const units::FPS kFps = 60;
const units::MS kMaxFrameTime = 5 * 1000 / 60;
}

//static
units::Tile Game::kScreenWidth = 20;
//static
units::Tile Game::kScreenHeight = 15;

Game::Game() {
   srand(static_cast<unsigned int>(time(NULL)));
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

   ParticleTools particle_tools = { front_particle_system_, entity_particle_system_, graphics };
   player_.reset(new Player(graphics, particle_tools, units::tileToGame(kScreenWidth / 2), units::tileToGame(kScreenHeight / 2)));
   damage_texts_.addDamageable(player_);
   bat_.reset(new FirstCaveBat(graphics, units::tileToGame(7), units::tileToGame(kScreenHeight / 2 + 1)));
   damage_texts_.addDamageable(bat_);
   map_.reset(Map::createSlopeTestMap(graphics));

   for (int i = 0; i < 3; ++i) {
      pickups_.add(boost::shared_ptr<Pickup>(new PowerDoritoPickup(
                  graphics,
                  bat_->center_x(), bat_->center_y(),
                  PowerDoritoPickup::MEDIUM)));
   }

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

      // Player Fire
      if (input.wasKeyPressed(SDLK_x)) {
         player_->startFire();
      } else if (input.wasKeyReleased(SDLK_x)) {
         player_->stopFire();
      }

      // Player Jump
      if (input.wasKeyPressed(SDLK_z)) {
         player_->startJump();
      } else if (input.wasKeyReleased(SDLK_z)) {
         player_->stopJump();
      }

      const units::MS current_time_ms = SDL_GetTicks();
      const units::MS elapsed_time = current_time_ms - last_update_time;
      update(std::min(elapsed_time, kMaxFrameTime), graphics);
      last_update_time = current_time_ms;

      draw(graphics);
      const units::MS ms_per_frame = 1000/*ms*/ / kFps;
      const units::MS elapsed_time_ms = SDL_GetTicks() - start_time_ms;
      if (elapsed_time_ms < ms_per_frame) {
         SDL_Delay(ms_per_frame - elapsed_time_ms);
      }
   }
}

void Game::update(units::MS elapsed_time_ms, Graphics& graphics) {
   Timer::updateAll(elapsed_time_ms);
   damage_texts_.update(elapsed_time_ms);
   pickups_.update(elapsed_time_ms, *map_);
   front_particle_system_.update(elapsed_time_ms);
   entity_particle_system_.update(elapsed_time_ms);

   player_->update(elapsed_time_ms, *map_);
   if (bat_) {
      if (!bat_->update(elapsed_time_ms, player_->center_x())) {
         ParticleTools particle_tools = { front_particle_system_, entity_particle_system_, graphics };
         DeathCloudParticle::createRandomDeathClouds(particle_tools,
            bat_->center_x(), bat_->center_y(),
            3);
         pickups_.add(FlashingPickup::heartPickup(
                  graphics, bat_->center_x(), bat_->center_y()));
         bat_.reset();
      }
   }

   std::vector<boost::shared_ptr<Projectile> > projectiles(player_->getProjectiles());
   for (size_t i = 0; i < projectiles.size(); ++i) {
      if (bat_ && bat_->collisionRectangle().collidesWith(projectiles[i]->collisionRectangle())) {
         bat_->takeDamage(projectiles[i]->contactDamage());
         projectiles[i]->collideWithEnemy();
      }
   }

   pickups_.handleCollision(*player_);

   if (bat_ && bat_->damageRectangle().collidesWith(player_->damageRectangle())) {
      player_->takeDamage(bat_->contactDamage());
   }
}

void Game::draw(Graphics& graphics) {
   graphics.clear();

   map_->drawBackground(graphics);
   if (bat_)
      bat_->draw(graphics);
   entity_particle_system_.draw(graphics);
   pickups_.draw(graphics);
   player_->draw(graphics);
   map_->draw(graphics);
   front_particle_system_.draw(graphics);

   damage_texts_.draw(graphics);
   player_->drawHUD(graphics);

   graphics.flip();
}
