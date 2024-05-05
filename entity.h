#pragma once
#include "terminal.h"
#include <vector>

class Entity {
protected:
    std::vector<Entity *> children;
public:
    virtual ~Entity();
    virtual void draw(Terminal *terminal);
    virtual void input(Key key_pressed);
    virtual void update(float delta_time);
};
