#include "projectile.h"

const std::string projectile_buffer = "^";

Projectile::Projectile(float x, float y, float velocity) : StringSprite(projectile_buffer, x, y) {
    this->velocity.y = velocity;
}

void Projectile::update(float delta_time) {
    y -= velocity.y * delta_time;
}
