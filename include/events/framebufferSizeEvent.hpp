#pragma once
#include "event.hpp"

struct FramebufferSizeEvent : public Event<true> {
    int width;
    int height;

    inline FramebufferSizeEvent(int width, int height)
        : width(width), height(height) {
    }
};
