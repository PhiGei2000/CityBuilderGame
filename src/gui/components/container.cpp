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
    Widget::render();

    for (const Widget* child : children) {
        child->render();
    }
}
