#include "polar_star.h"

#include "sprite.h"
#include "map.h"
#include "particle_system.h"
#include "projectile_star_particle.h"

namespace {
const int kPolarStarIndex = 2; // 0-based indexing
const units::Game kGunWidth = 3 * units::kHalfTile;
const units::Game kGunHeight = 2 * units::kHalfTile;

const units::Tile kHorizontalOffset = 0;
const units::Tile kUpOffset = 2;
const units::Tile kDownOffset = 4;

const units::Tile kLeftOffset = 0;
const units::Tile kRightOffset = 1;

const std::string kSpritePath("Arms");

// Nozzle Offsets
const units::Game kNozzleHorizontalY = 23;
const units::Game kNozzleHorizontalLeftX = 10;
const units::Game kNozzleHorizontalRightX = 38;

const units::Game kNozzleUpY = 4;
const units::Game kNozzleUpLeftX = 27;
const units::Game kNozzleUpRightX = 21;

const units::Game kNozzleDownY = 28;
const units::Game kNozzleDownLeftX = 29;
const units::Game kNozzleDownRightX = 19;

// Projectile Sprite
const units::Tile kProjectileSourceY = 2;
const units::Tile kHorizontalProjectileSourceX = 8;
const units::Tile kVerticalProjectileSourceX = 9;
// Projectile Velocity
const units::Velocity kProjectileSpeed = 0.6f;

const units::Game kProjectileMaxOffset = 7 * units::kHalfTile;
const units::Game kProjectileWidth = 4.0f;
}

PolarStar::PolarStar(Graphics& graphics) {
   initializeSprites(graphics);
}

void PolarStar::updateProjectiles(units::MS elapsed_time, const Map& map, ParticleTools& particle_tools) {
   if (projectile_a_) {
      if (!projectile_a_->update(elapsed_time, map, particle_tools)) {
         projectile_a_.reset();
      }
   }
   if (projectile_b_) {
      if (!projectile_b_->update(elapsed_time, map, particle_tools)) {
         projectile_b_.reset();
      }
   }
}

void PolarStar::draw(
      Graphics& graphics,
      HorizontalFacing horizontal_facing, VerticalFacing vertical_facing,
      bool gun_up,
      units::Game player_x, units::Game player_y) {
   units::Game x = gun_x(horizontal_facing, player_x);
   units::Game y = gun_y(vertical_facing, gun_up, player_y);

   sprite_map_[boost::make_tuple(horizontal_facing, vertical_facing)]->draw(graphics, x, y);
   if (projectile_a_)
      projectile_a_->draw(graphics);
   if (projectile_b_)
      projectile_b_->draw(graphics);
}

units::Game PolarStar::gun_y(VerticalFacing vertical_facing, bool gun_up, units::Game player_y) const {
   units::Game gun_y = player_y;
   if (vertical_facing == UP)
      gun_y -= units::kHalfTile / 2;
   if (vertical_facing == DOWN)
      gun_y += units::kHalfTile / 2;
   if (gun_up)
      gun_y -= 2.0f;
   return gun_y;
}

void PolarStar::startFire(units::Game player_x, units::Game player_y,
                          HorizontalFacing horizontal_facing,
                          VerticalFacing vertical_facing,
                          bool gun_up, ParticleTools& particle_tools) {
   if (projectile_a_ && projectile_b_) return;

   units::Game bullet_x = gun_x(horizontal_facing, player_x) - units::kHalfTile;
   units::Game bullet_y = gun_y(vertical_facing, gun_up, player_y) - units::kHalfTile;
   switch (vertical_facing) {
      case HORIZONTAL:
         bullet_y += kNozzleHorizontalY;
         if (horizontal_facing == LEFT) {
            bullet_x += kNozzleHorizontalLeftX;
         } else {
            bullet_x += kNozzleHorizontalRightX;
         }
         break;
      case UP:
         bullet_y += kNozzleUpY;
         if (horizontal_facing == LEFT) {
            bullet_x += kNozzleUpLeftX;
         } else {
            bullet_x += kNozzleUpRightX;
         }
         break;
      case DOWN:
         bullet_y += kNozzleDownY;
         if (horizontal_facing == LEFT) {
            bullet_x += kNozzleDownLeftX;
         } else {
            bullet_x += kNozzleDownRightX;
         }
         break;
      default: break;
   }
   // Create our projectile using bullet_x, bullet_y
   if (!projectile_a_) {
      projectile_a_.reset(new Projectile(
         vertical_facing == HORIZONTAL ? horizontal_projectile_ : vertical_projectile_,
         horizontal_facing, vertical_facing,
         bullet_x, bullet_y,
         particle_tools));
   } else if (!projectile_b_) {
      projectile_b_.reset(new Projectile(
         vertical_facing == HORIZONTAL ? horizontal_projectile_ : vertical_projectile_,
         horizontal_facing, vertical_facing,
         bullet_x, bullet_y,
         particle_tools));
   }
}

std::vector<boost::shared_ptr<Projectile> > PolarStar::getProjectiles() {
   std::vector<boost::shared_ptr< ::Projectile> > projectiles;
   if (projectile_a_) projectiles.push_back(projectile_a_);
   if (projectile_b_) projectiles.push_back(projectile_b_);
   return projectiles;
}

void PolarStar::initializeSprites(Graphics& graphics) {
   horizontal_projectile_.reset(new Sprite(
      graphics, "Bullet",
      units::tileToPixel(kHorizontalProjectileSourceX), units::tileToPixel(kProjectileSourceY),
      units::tileToPixel(1), units::tileToPixel(1)));
   vertical_projectile_.reset(new Sprite(
      graphics, "Bullet",
      units::tileToPixel(kVerticalProjectileSourceX), units::tileToPixel(kProjectileSourceY),
      units::tileToPixel(1), units::tileToPixel(1)));
   ENUM_FOREACH(h_facing, HORIZONTAL_FACING) {
      ENUM_FOREACH(v_facing, VERTICAL_FACING) {
         initializeSprite(graphics, boost::make_tuple((HorizontalFacing)h_facing,
                                                      (VerticalFacing)v_facing));
      }
   }
}

void PolarStar::initializeSprite(Graphics& graphics, const SpriteState& sprite_state) {
   units::Tile tile_y = sprite_state.horizontal_facing() == LEFT ? kLeftOffset : kRightOffset;
   switch (sprite_state.vertical_facing()) {
      case HORIZONTAL:
         tile_y += kHorizontalOffset;
         break;
      case UP:
         tile_y += kUpOffset;
         break;
      case DOWN:
         tile_y += kDownOffset;
         break;
      case LAST_VERTICAL_FACING: break;
   }
   sprite_map_[sprite_state] = boost::shared_ptr<Sprite>(new Sprite(graphics, kSpritePath,
      units::gameToPixel(kPolarStarIndex * kGunWidth), units::tileToPixel(tile_y),
      units::gameToPixel(kGunWidth), units::gameToPixel(kGunHeight)));
}

PolarStar::Projectile::Projectile(boost::shared_ptr<Sprite> sprite,
                                  HorizontalFacing horizontal_direction,
                                  VerticalFacing vertical_direction,
                                  units::Game x, units::Game y,
                                  ParticleTools& particle_tools) :
   sprite_(sprite),
   horizontal_direction_(horizontal_direction),
   vertical_direction_(vertical_direction),
   x_(x), y_(y),
   offset_(0),
   alive_(true)
{
   particle_tools.system.addNewParticle(boost::shared_ptr<Particle>(
      new ProjectileStarParticle(particle_tools.graphics, x, y)));
}

bool PolarStar::Projectile::update(units::MS elapsed_time, const Map& map, ParticleTools& particle_tools) {
   offset_ += kProjectileSpeed * elapsed_time;

   std::vector<Map::CollisionTile> colliding_tiles(
      map.getCollidingTiles(collisionRectangle()));
   for (size_t i = 0; i < colliding_tiles.size(); ++i) {
      if (colliding_tiles[i].tile_type == Map::WALL_TILE) {
         return false;
      }
   }

   if (!alive_) {
      return false;
   } else if (offset_ >= kProjectileMaxOffset) {
      particle_tools.system.addNewParticle(boost::shared_ptr<Particle>(
         new ProjectileStarParticle(particle_tools.graphics, getX(), getY())));
      return false;
   } else {
      return true;
   }
}

Rectangle PolarStar::Projectile::collisionRectangle() const {
   const units::Game width = vertical_direction_ == HORIZONTAL ?
      units::tileToGame(1) : kProjectileWidth;
   const units::Game height = vertical_direction_ != HORIZONTAL ?
      units::tileToGame(1) : kProjectileWidth;
   return Rectangle(getX() + units::kHalfTile - width  / 2,
                    getY() + units::kHalfTile - height / 2,
                    width, height);
}

units::Game PolarStar::Projectile::getX() const {
   if (vertical_direction_ == HORIZONTAL)
      return x_ + (horizontal_direction_ == LEFT ? -offset_ : offset_);
   return x_;
}

units::Game PolarStar::Projectile::getY() const {
   units::Game y = y_;
   switch (vertical_direction_) {
      case UP:
         y -= offset_;
         break;
      case DOWN:
         y += offset_;
         break;
      default: break;
   }
   return y;
}

void PolarStar::Projectile::draw(Graphics& graphics) {
   sprite_->draw(graphics, getX(), getY());
}