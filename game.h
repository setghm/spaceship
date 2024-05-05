#pragma once

#include "terminal.h"3
#include "player.h"
#include "asteroids.h"
#include <vector>
#include <chrono>

#define DEFAULT_LABEL_BLINK_TIME    0.5f

class Game {
    private:
        // Internal fields
        Terminal terminal = Terminal();
        bool quit = false;
        float last_update;

        // Game state
        bool game_over = false;
        unsigned int high_score;
        float label_blink_timer = DEFAULT_LABEL_BLINK_TIME;
        bool show_label_blink = true;

        // Game objects
        Player *player;
        Asteroids *asteroids;

    public:
        Game();
        ~Game();
        int run();

    private:
        // Internal lifecicle methods
        void update();
        void input();
        void draw();

        // Game lifecicle methods
        void restart();
};


