#pragma once
#include "terminal.h"
#include "string_sprite.h"
#include "asteroids.h"
#include <vector>

class Player : public StringSprite {
    private:
        unsigned int score;
        bool dead;
    private:
        void shoot();
        void remove_outside_projectiles();
        bool colliding_with_asteroid(Asteroid *asteroid);
    public:
        Player(int maxx, int maxy);
        virtual void input(Key key);
        virtual void update(float delta_time);
        std::vector<int> check_asteroid_collisions(std::vector<Asteroid *> asteroids);

        void initState(int maxx, int maxy);

        // Accessors
        int getScore() { return score; }
        bool isDead() { return dead; }
};


