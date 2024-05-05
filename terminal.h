#pragma once
#include <termios.h>
#include <cstring>
#include <string>

enum Key {
    None,
    LeftArrow,
    RightArrow,
    UpArrow,
    DownArrow,
    Spacebar,
    KeyW,
    KeyA,
    KeyD,
    KeyS,
    KeyF,
    Escape,
    Enter
};

class Terminal {
    private:
        struct termios original_data;
        int old_flags;
    public:
        static int width;
        static int height;

        Terminal();
        ~Terminal();
        void move(int x, int y);
        void clear();
        int read_key();
        void draw_buffer(std::string buffer, int x, int y);
};
