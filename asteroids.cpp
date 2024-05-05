#include "asteroids.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

const std::string asteroid_buffer = "@";

Asteroid::Asteroid(float x, float y, float velocity) : StringSprite(asteroid_buffer, x, y) {
    this->velocity.y = velocity;
}

void Asteroid::update(float delta_time) {
    y += delta_time * velocity.y;
}

Asteroids::Asteroids(int maxx, int maxy) {
    x_range.min = 0;
    x_range.max = maxx;
    y_range.min = 0;
    y_range.max = maxy;

    last_x_pos = -1;
    generating_asteroids = true;

    start_timer();
}

void Asteroids::update(float delta_time) {
    if (generating_asteroids) {
        // Update the timer
        timer -= delta_time;

        // Add new asteroid
        generate_asteroids();
    }

    // Delete asteroids that are outside of the world
    remove_outside_asteroids();

    // Update all the asteroids
    Entity::update(delta_time);
}

void Asteroids::start_timer() {
    float start_ms = rand() % (MAX_GEN_INTERVAL - MIN_GEN_INTERVAL + 1) + MIN_GEN_INTERVAL;
    timer = start_ms / 1000;
}

void Asteroids::generate_asteroids() {
    if (timer > 0) return;

    start_timer();

    // Avoid repeating the same position
    float x = 0;
    do {
        x = rand() % (x_range.max - x_range.min + 1) + x_range.min;
    } while (x == last_x_pos);

    last_x_pos = x;

    float v = rand() % (MAX_VELOCITY - MIN_VELOCITY + 1) + MIN_VELOCITY;

    Asteroid *asteroid = new Asteroid(x, 1, v);
    children.push_back(asteroid);
}

void Asteroids::remove_outside_asteroids() {
    // Delete asteroids that are outside of the world
    children.erase(std::remove_if(begin(children), end(children), [this](Entity *child){
        Asteroid *asteroid = dynamic_cast<Asteroid *>(child);

        if (asteroid->y > y_range.max) {
            delete asteroid;
            return true;
        }

        return false;
    }), end(children));
}

std::vector<Asteroid *> Asteroids::getAll() {
    std::vector<Asteroid *> asteroids;

    for (Entity *child : children) {
        Asteroid *asteroid = dynamic_cast<Asteroid *>(child);
        asteroids.push_back(asteroid);
    }

    return asteroids;
}

void Asteroids::remove_colliding_asteroids(std::vector<int> indexes) {
    // Free the asteroids that are in the indexes vector
    for (int index : indexes) {
        delete children[index];
        children[index] = nullptr;
    }

    // Remove the nullptr children
    children.erase(std::remove_if(children.begin(), children.end(),
                                  [](Entity *child){ return child == nullptr; }),
                                  children.end());
}
