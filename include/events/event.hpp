#pragma once
#include <concepts>

template<bool ignoreState = false>
struct Event {
    bool handled = false;

    inline Event()
        : handled(false) {
    }
};

template<typename T, bool ignoreState>
concept EventType = std::derived_from<T, Event<ignoreState>>;
