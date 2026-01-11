#pragma once
#include "gui/components/widget.hpp"

#include "events/mouseEvents.hpp"

#include <optional>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#undef GLFW_INCLUDE_NONE

struct MenuBase {

    inline void onShow(Gui* gui) const {
    }

    inline virtual constexpr std::optional<int> getKey() const {
        return {};
    }

    inline virtual constexpr bool toggleOnKey() const {
        return false;
    }

    virtual Widget* getWidget() = 0;
};

template<WidgetType T>
struct Menu : public T, public MenuBase {

    template<typename... TArgs>
    inline Menu(TArgs... args) : T(args...) {
    }

    inline virtual Widget* getWidget() {
        return static_cast<T*>(this);
    }
};
