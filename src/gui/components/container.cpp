#include "gui/components/container.hpp"

#include "events/mouseEvents.hpp"

#include <sstream>

Container::Container(const std::string& id, Gui* gui, const glm::vec4& backgroundColor)
    : Widget(id, gui, backgroundColor) {
}

void Container::applyConstraints() {
    Widget::applyConstraints();

    for (Widget* child : children) {
        child->applyConstraints();
    }
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

void Container::invalidate() {
    Widget::invalidate();

    for (Widget* child : children) {
        child->invalidate();
    }
}

void Container::update() {
    for (Widget* child : children) {
        child->update();
    }
}

void Container::render() {
    if (!visible) {
        return;
    }

    Widget::render();

    for (Widget* child : children) {
        child->render();
    }
}

void Container::setChildConstraints() {
    for (const auto& child : children) {
        Container* container = dynamic_cast<Container*>(child);
        if (container != nullptr) {
            container->setChildConstraints();
        }

        child->invalidate();
        child->applyConstraints();
    }
}
