#pragma once

struct Event {
    bool handled = false;

    inline Event()
        : handled(false) {
    }
};
