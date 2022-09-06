#include "gui/components/button.hpp"

#include "events/mouseEvents.hpp"

#include <GLFW/glfw3.h>

Button::Button(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text)
    : Label(id, gui, backgroundColor, text) {
}

void Button::handleMouseButtonEvent(const MouseButtonEvent& event) {
    if (!visible)
        return;

    const Rectangle& area = getBox();

    if (!area.pointInside(event.x, event.y)) {
        return;
    }

    if (event.action == GLFW_PRESS && event.button == GLFW_MOUSE_BUTTON_LEFT) {
        onClick.invoke(event);
    }
}
