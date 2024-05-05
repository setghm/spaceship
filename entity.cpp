#include "entity.h"

Entity::~Entity() {
    for (Entity *child : children) {
        if (child == nullptr) {
            continue;
        }
        delete child;
    }
}

void Entity::draw(Terminal *terminal) {
    for (Entity *child : children) {
        child->draw(terminal);
    }
}

void Entity::update(float delta_time) {
    for (Entity *child : children) {
        child->update(delta_time);
    }
}

void Entity::input(Key key_pressed) {
    for (Entity *child : children) {
        child->input(key_pressed);
    }
}
