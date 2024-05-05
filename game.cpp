#include "game.h"
#include "entity.h"
#include <cstdio>
#include <unistd.h>
#include <ctime>
#include <cstdlib>

Game::Game() {
    srand(time(nullptr));

    high_score = 0;

    player = new Player(terminal.width, terminal.height);
    asteroids = new Asteroids(terminal.width, terminal.height);
}

Game::~Game() {
    delete player;
    delete asteroids;
}

void Game::restart() {
    player->initState(terminal.width, terminal.height);
    asteroids->startGenerating();
    game_over = false;
}

void Game::input() {
    Key k;
    int c = terminal.read_key();

    switch (c) {
        case 27: {
            c = terminal.read_key();

            if (c == 91) {
                c = terminal.read_key();

                switch (c) {
                    case 65: { k = Key::UpArrow; break; }
                    case 66: { k = Key::DownArrow; break; }
                    case 67: { k = Key::RightArrow; break; }
                    case 68: { k = Key::LeftArrow; break; }
                }
            } else if (c == -1) {
                k = Key::Escape;
            }
            break;
        }
        case 13: { k = Key::Enter; break; }
        case 32: { k = Key::Spacebar; break; }
        case 65: case 97:  { k = Key::KeyA; break; }
        case 68: case 100: { k = Key::KeyD; break; }
        case 70: case 102: { k = Key::KeyF; break; }
        case 83: case 115: { k = Key::KeyS; break; }
        case 87: case 119: { k = Key::KeyW; break; }
        default: { k = Key::None; }
    }

    // Quit if the ESCAPE key was pressed
    if (k == Key::Escape) {
        quit = true;
        return;
    }

    if (player->isDead()) {
        // Restart when the user press the spacebar key
        if (k == Key::Enter) {
            restart();
        }
    } else {
        // Only the player needs to catch the input
        player->input(k);
    }
}

void Game::update() {
    // Calculate delta time
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    float current_time = (t.tv_nsec / 1E+9 + t.tv_sec);
    float delta_time = current_time - last_update;
    last_update = current_time;

    asteroids->update(delta_time);

    if (!game_over) {
        // Update the player only if it isn't dead
        player->update(delta_time);

        // Check collisions with asteroids
        std::vector<int> indexes = player->check_asteroid_collisions(asteroids->getAll());
        asteroids->remove_colliding_asteroids(indexes);

        // Check if the player is dead
        if (player->isDead()) {
            game_over = true;
            asteroids->stopGenerating();
        }
    } else {
        // Update the timer for the label blink
        label_blink_timer -= delta_time;

        if (label_blink_timer <= 0) {
            // Restart the blink timer
            label_blink_timer = DEFAULT_LABEL_BLINK_TIME;
            // Toggle label visibility
            show_label_blink = !show_label_blink;
        }
    }
}

void Game::draw() {
    terminal.clear();

    asteroids->draw(&terminal);

    if (!game_over) {
        // Draw the player only if it isn't dead
        player->draw(&terminal);

        // Draw the score and highscore
        int score = player->getScore();

        terminal.move(0, 0);
        printf("Score: %d", score);

        if (score > high_score) {
            high_score = score;
        }

        if (high_score != 0) {
            std::string high_score_str = "High Score: " + std::to_string(high_score);
            terminal.move(terminal.width - high_score_str.length(), 0);
            puts(high_score_str.c_str());
        }
    } else {
        // Draw the highscore and the last score at the center of the screen
        std::string game_over_str = "GAME OVER";
        std::string high_score_str = "High Score: " + std::to_string(high_score);
        std::string last_score_str = "Last Score: " + std::to_string(player->getScore());

        terminal.move((int) (terminal.width - game_over_str.length()) / 2,
                      (int) (terminal.height / 2) - 2);
        puts(game_over_str.c_str());

        terminal.move((int) (terminal.width - high_score_str.length()) / 2,
                      (int) (terminal.height / 2) + 1);
        puts(high_score_str.c_str());

        terminal.move((int) (terminal.width - last_score_str.length()) / 2,
                      (int) (terminal.height / 2) + 2);
        puts(last_score_str.c_str());

        if (show_label_blink) {
            std::string restart_str = "< ENTER to restart >";

            // Draw the blink label
            terminal.move((int) (terminal.width - restart_str.length()) / 2,
                          (int) (terminal.height / 2) + 4);
            puts(restart_str.c_str());
        }
    }
}

int Game::run() {
    while(!quit) {
        input();
        update();
        draw();
        usleep(100);
    }

    return 0;
}

