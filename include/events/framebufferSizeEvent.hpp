#pragma once
#include "event.hpp"

struct FramebufferSizeEvent : public Event {
    int width;
    int height;

    inline FramebufferSizeEvent(int width, int height)
        : width(width), height(height) {
    }
};