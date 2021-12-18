#pragma once
#include "event.hpp"

struct FramebufferSizeEvent : public Event {
    int width;
    int height;
};