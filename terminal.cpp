#include "terminal.h"
#include <cstdio>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

int Terminal::width = 80;
int Terminal::height = 20;

Terminal::Terminal() {
    struct termios new_data;

    // Initialize terminal non-blocking mode and
    // remove echo
    old_flags = fcntl(0, F_GETFL);
    fcntl(0, F_SETFL, O_NONBLOCK);

    tcgetattr(0, &original_data);
    memcpy(&new_data, &original_data, sizeof(struct termios));

    cfmakeraw(&new_data);
    tcsetattr(0, TCSANOW, &new_data);

    // Get terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    Terminal::width = w.ws_col;
    Terminal::height = w.ws_row;

    // Hide the cursor
    printf("\033[?25l");

}

Terminal::~Terminal() {
    // Clear all
    clear();

    // Reset terminal to original attributes
    tcsetattr(0, TCSANOW, &original_data);
    fcntl(0, F_SETFL, old_flags);

    // Show the cursor
    printf("\033[?25h");
}

void Terminal::move(int x, int y) {
    printf("%c[%d;%df", 0x1B, y, x);
}

void Terminal::clear() {
    printf("\033c");
}

int Terminal::read_key() {
    return getchar();
}

void Terminal::draw_buffer(std::string buffer, int x, int y) {
    int length = buffer.length();
    move(x, y);

    for (int i=0; i<length; i++) {
        if (buffer.at(i) == '\n') {
            y += 1;
            move(x, y);
        } else {
            putchar(buffer.at(i));
        }
    }
}
