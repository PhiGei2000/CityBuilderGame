#include "gui/components/button.hpp"

#include "events/mouseEvents.hpp"

#include <GLFW/glfw3.h>

Button::Button(const std::string& id, Gui* gui, const glm::vec4& backgroundColor)
    : Widget(id, gui, backgroundColor) {
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

TextButton::TextButton(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text)
    : Label(id, gui, backgroundColor, text), Widget(id, gui, backgroundColor), Button(id, gui, backgroundColor) {
}

IconButton::IconButton(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, Texture* icon)
    : Icon(id, gui, icon, backgroundColor),
      Widget(id, gui, backgroundColor),
      Button(id, gui, backgroundColor) {
}
