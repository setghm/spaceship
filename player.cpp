#include "player.h"
#include "projectile.h"
#include "entity.h"
#include <algorithm>

const std::string player_buffer =
    "_/^^\\_""\n"
    "v----v";

Player::Player(int maxx, int maxy) : StringSprite(player_buffer, 0, 0) {
    // Set the desired velocity in x-y
    velocity.x = maxx / 30;
    velocity.y = maxy / 12;

    // Minimum velocity in X axis is 3
    if (velocity.x < 3) velocity.x = 3;

    initState(maxx, maxy);
}

void Player::initState(int maxx, int maxy) {
    // Position the spaceship at center-bottom
    x = (maxx - width) / 2;
    y = maxy - 2;

    score = 0;
    dead = false;
}

void Player::input(Key key) {
    bool up = key == Key::UpArrow || key == Key::KeyW;
    bool down = key == Key::DownArrow || key == Key::KeyS;
    bool left = key == Key::LeftArrow || key == Key::KeyA;
    bool right = key == Key::RightArrow || key == Key::KeyD;
    bool fire = key == Key::Spacebar || key == Key::KeyF;

    if (up && (y - velocity.y) > 0) {
        y -= velocity.y;
    }
    else if (down && (y + velocity.y + height - 1) <= Terminal::height) {
        y += velocity.y;
    }
    else if (left && (x - velocity.x) > 0) {
        x -= velocity.x;
    }
    else if (right && (x + velocity.x + width - 1) <= Terminal::width) {
        x += velocity.x;
    }
    else if (fire) {
        shoot();
    }
}

void Player::shoot() {
    // Calculate the position where the projectile will appear
    float proj_y  = y - 1;
    float proj1_x = x + 2;
    float proj2_x = x + 3;

    // Create two projectiles and add
    Projectile *projectile1 = new Projectile(proj1_x, proj_y, 30);
    Projectile *projectile2 = new Projectile(proj2_x, proj_y, 30);

    children.push_back(projectile1);
    children.push_back(projectile2);
}

void Player::update(float delta_time) {
    Entity::update(delta_time);

    if (children.size() > 0) {
        remove_outside_projectiles();
    }
}

void Player::remove_outside_projectiles() {
    // Delete projectiles that are outside of the world
    children.erase(std::remove_if(begin(children), end(children), [](Entity *child){
        Projectile *projectile = dynamic_cast<Projectile *>(child);

        if (projectile->y < 0) {
            delete projectile;
            return true;
        }

        return false;
    }), end(children));
}

// Check if there are collisions with an asteroid and returns a vector
// with the indexes of the colliding asteroids
std::vector<int> Player::check_asteroid_collisions(std::vector<Asteroid *> asteroids) {
    std::vector<int> colliding_asteroids;

    // Check if an asteroid is colliding with the spaceship
    for (int i=0; i<asteroids.size(); i++) {
        if (colliding_with_asteroid(asteroids.at(i))) {
            colliding_asteroids.push_back(i);
            dead = true;
        }
    }

    // Delete projectiles that are colliding with an asteroid
    auto pred = [&colliding_asteroids, asteroids, this](Entity *child){
        Projectile *projectile = dynamic_cast<Projectile *>(child);

        for (int i=0; i<asteroids.size(); i++) {
            // Only check if are in the same position since
            // both objects has 1x1 dimensions
            int x1 = (int)asteroids.at(i)->x;
            int y1 = (int)asteroids.at(i)->y;
            int x2 = (int)projectile->x;
            int y2 = (int)projectile->y;

            if (x1 == x2 && y1 == y2) {
                score++;
                colliding_asteroids.push_back(i);
                delete projectile;
                return true;
            }
        }

        return false;
    };

    children.erase(std::remove_if(begin(children), end(children), pred), end(children));

    return colliding_asteroids;
}

bool Player::colliding_with_asteroid(Asteroid *asteroid) {
    // Only check if the asteroid is inside the
    // collision box of the player since its dimensions are 1x1
    int x1  = (int) asteroid->x;
    int y1  = (int) asteroid->y;
    int x2i = (int) x;
    int x2f = (int) (x + width);
    int y2i = (int) y;
    int y2f = (int) (y + height);

    if (x1 >= x2i && x1 < x2f && y1 >= y2i && y1 < y2f) {
        return true;
    }

    return false;
}
