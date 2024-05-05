#pragma once
#include "string_sprite.h"
#include "entity.h"

#define MIN_VELOCITY        10
#define MAX_VELOCITY        15
#define MIN_GEN_INTERVAL    100
#define MAX_GEN_INTERVAL    500

class Asteroid : public StringSprite {
    public:
        Asteroid(float x, float y, float velocity);
        virtual void update(float delta_time);
};

class Asteroids : public Entity {
    private:
        struct {
            int min;
            int max;
        } x_range;
        struct {
            int min;
            int max;
        } y_range;
        float last_x_pos;
        float timer;
        void start_timer();
        void generate_asteroids();
        void remove_outside_asteroids();
        bool generating_asteroids;
    public:
        Asteroids(int maxx, int maxy);
        virtual void update(float delta_time);
        std::vector<Asteroid *> getAll();
        void remove_colliding_asteroids(std::vector<int> indexes);

        // accessors
        void stopGenerating() { generating_asteroids = false; }
        void startGenerating() { generating_asteroids = true; }
};

