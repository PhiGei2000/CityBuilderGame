#include "gui/components/container.hpp"

#include "events/mouseEvents.hpp"

#include <sstream>

ContainerItem::ContainerItem(const std::string& id, Gui* gui, Widget* element, const glm::vec4 backgroundColor)
    : Widget(id, gui, backgroundColor), element(element) {
    element->parent = this;
}

void ContainerItem::handleMouseButtonEvent(const MouseButtonEvent& e) {
    if (!visible)
        return;

    const Rectangle& area = getBox();
    if (area.pointInside(e.x, e.y)) {
        element->handleMouseButtonEvent(e);
    }
}

void ContainerItem::handleMouseMoveEvent(const MouseMoveEvent& e) {
    if (!visible)
        return;

    element->handleMouseMoveEvent(e);
}

void ContainerItem::show() {
    Widget::show();

    element->show();
}

void ContainerItem::hide() {
    Widget::hide();

    element->hide();
}

void ContainerItem::render() const {
    Widget::render();

    element->render();
}

const Widget* ContainerItem::getElementAt(float x, float y) const {
    const Widget* base = Widget::getElementAt(x, y);

    if (base == nullptr)
        return nullptr;

    return element->getElementAt(x, y);
}

Container::Container(const std::string& id, Gui* gui, const glm::vec4& backgroundColor)
    : Widget(id, gui, backgroundColor) {
}

void Container::handleMouseButtonEvent(const MouseButtonEvent& e) {
    if (!visible)
        return;

    for (Widget* child : children) {
        child->handleMouseButtonEvent(e);
    }
}

void Container::handleMouseMoveEvent(const MouseMoveEvent& e) {
    if (!visible)
        return;

    for (Widget* child : children) {
        child->handleMouseMoveEvent(e);
    }
}

void Container::addChild(Widget* child) {
    std::stringstream ss;
    ss << id << "Item" << children.size();

    ContainerItem* item = new ContainerItem(ss.str(), gui, child);
    item->parent = this;

    children.push_back(item);

    setChildConstraints();
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

void Container::render() const {
    Widget::render();

    for (const Widget* child : children) {
        child->render();
    }
}

const Widget* Container::getElementAt(float x, float y) const {
    const Rectangle& area = getBox();

    if (area.pointInside(x, y)) {
        for (auto it = children.begin(); it != children.end(); it++) {
            const Widget* element = (*it)->getElementAt(x, y);
            if (element != nullptr) {
                return element;
            }
        }
    }

    return nullptr;
}