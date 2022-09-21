#pragma once
#include "event.hpp"

struct KeyEvent : public Event {
    int key;
    int scancode;
    int action;
    int mods;

    inline KeyEvent(int key, int scancode, int action, int mods)
        : key(key), scancode(scancode), action(action), mods(mods) {
    }
};