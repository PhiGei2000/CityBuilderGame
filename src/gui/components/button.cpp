/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "gui/components/button.hpp"

#include "events/mouseEvents.hpp"

#include <GLFW/glfw3.h>

Button::Button(const std::string& id, Gui* gui, const glm::vec4& backgroundColor)
    : Widget(id, gui, backgroundColor) {

    // hover effect
    onMouseEnter += [&](const MouseMoveEvent& e) {
        this->backgroundColor = colors::white;
    };
    onMouseLeave += [&](const MouseMoveEvent& e) {
        this->backgroundColor = colors::anthraziteGrey;
    };
}

void Button::handleMouseButtonEvent(MouseButtonEvent& event) {
    if (!visible)
        return;

    const Rectangle& area = getBox();

    if (!area.pointInside(event.x, event.y)) {
        return;
    }

    if (event.action == GLFW_RELEASE && event.button == GLFW_MOUSE_BUTTON_LEFT) {
        onClick.invoke(event);
    }
}

TextButton::TextButton(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, int textSize, const glm::vec4& textColor)
    : Label(id, gui, backgroundColor, text, textSize, textColor), Widget(id, gui, backgroundColor), Button(id, gui, backgroundColor) {

    // hover effect text color
    onMouseEnter += [&](const MouseMoveEvent& e) {
        Label::textColor = colors::black;
    };
    onMouseLeave += [&](const MouseMoveEvent& e) {
        Label::textColor = colors::white;
    };
}

IconButton::IconButton(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, Texture* icon)
    : Icon(id, gui, icon, backgroundColor),
      Widget(id, gui, backgroundColor),
      Button(id, gui, backgroundColor) {
}
