#include "string_sprite.h"

StringSprite::StringSprite(std::string buffer, float x, float y) {
    this->x = x;
    this->y = y;
    width = 0;
    height = 0;
    velocity.x = 0;
    velocity.y = 0;

    // Calculate the dimensions of the buffer
    float w = 0;
    int len = buffer.length();
    this->buffer = buffer;

    // Add the first line
    if (len > 0) {
        height = 1;
    }

    // Count the number of lines and columns
    for (int i=0; i<len; i++) {
        if (buffer[i] == '\n') {
            if (w > width) {
                width = w;
            }
            w = 0;
            height++;

            continue;
        }

        w++;
    }
}

void StringSprite::draw(Terminal *terminal) {
    //if (x > 0 && (x + width) <= terminal->width && y > 0 && (y + height) <= terminal->height) {
    terminal->draw_buffer(buffer, (int)x, (int)y);
    //}

    // Draw children
    Entity::draw(terminal);
}

