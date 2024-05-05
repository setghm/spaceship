#pragma once
#include "entity.h"
#include "terminal.h"
#include <string>

class StringSprite : public Entity {
    public:
        float x;
        float y;
    protected:
        struct {
            float x;
            float y;
        } velocity;
        float width;
        float height;
        std::string buffer;
    public:
        StringSprite(std::string buffer, float x, float y);
        virtual void draw(Terminal *terminal);
};
