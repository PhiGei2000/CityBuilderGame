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
#include "gui/components/container.hpp"

#include "events/mouseEvents.hpp"

#include <sstream>

Container::Container(const std::string& id, Gui* gui, const glm::vec4& backgroundColor)
    : Widget(id, gui, backgroundColor) {
}

void Container::handleMouseButtonEvent(MouseButtonEvent& e) {
    if (!visible)
        return;

    for (Widget* child : children) {
        child->handleMouseButtonEvent(e);
    }
}

void Container::handleMouseMoveEvent(MouseMoveEvent& e) {
    if (!visible)
        return;

    for (Widget* child : children) {
        child->handleMouseMoveEvent(e);
    }
}

void Container::addChild(Widget* child) {
    children.push_back(child);
    child->parent = this;
    if (visible) {
        child->show();
    }
    else {
        child->hide();
    }

    setChildConstraints();
}

Widget* Container::getChild(const std::string& id) const {
    for (Widget* child : children) {
        if (child->id == id) {
            return child;
        }
    }

    return nullptr;
}

void Container::show() {
    Widget::show();

    for (Widget* child : children) {
        child->show();
    }
}

void Container::hide() {
    Widget::hide();

    for (Widget* child : children) {
        child->hide();
    }
}

void Container::update() {
    for (Widget* child : children) {
        child->update();
    }
}

void Container::render() const {
    if (!visible) {
        return;
    }

    Widget::render();

    for (const Widget* child : children) {
        child->render();
    }
}
