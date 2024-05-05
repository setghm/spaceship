#pragma once
#include "string_sprite.h"

class Projectile : public StringSprite {
    public:
        Projectile(float x, float y, float velocity);
        virtual void update(float delta_time);
        // TODO: implement collision detection with asteroids
        bool colliding_with() { return false; }
};
