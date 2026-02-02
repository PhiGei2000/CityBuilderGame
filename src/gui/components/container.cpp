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

Rectangle Container::getChildArea() {
    const Rectangle& box = getBox();
    Rectangle childArea = box;

    float dtop = 0;
    float dleft = 0;
    float dbottom = 0;
    float dright = 0;
    switch (padding.top.getType()) {
        case ConstraintType::ABSOLUTE:
            dtop = padding.top.getValue<AbsoluteConstraint>().value;
            break;
        case ConstraintType::RELATIVE:
            dtop = padding.top.getValue<RelativeConstraint>().value * box.height;
            break;
        default:
            throw std::runtime_error("Invalid padding constraint (top)");
    }

    switch (padding.left.getType()) {
        case ConstraintType::ABSOLUTE:
            dleft = padding.left.getValue<AbsoluteConstraint>().value;
            break;
        case ConstraintType::RELATIVE:
            dleft = padding.left.getValue<RelativeConstraint>().value * box.width;
            break;
        default:
            throw std::runtime_error("Invalid padding constraint (left)");
    }

    switch (padding.bottom.getType()) {
        case ConstraintType::ABSOLUTE:
            dbottom = padding.bottom.getValue<AbsoluteConstraint>().value;
            break;
        case ConstraintType::RELATIVE:
            dbottom = padding.bottom.getValue<RelativeConstraint>().value * box.height;
            break;
        default:
            throw std::runtime_error("Invalid padding constraint (bottom)");
    }

    switch (padding.right.getType()) {
        case ConstraintType::ABSOLUTE:
            dright = padding.right.getValue<AbsoluteConstraint>().value;
            break;
        case ConstraintType::RELATIVE:
            dright = padding.right.getValue<RelativeConstraint>().value * box.height;
            break;
        default:
            throw std::runtime_error("Invalid padding constraint (right)");
    }

    childArea.x += dleft;
    childArea.y += dtop;
    childArea.width -= (dleft + dright);
    childArea.height -= (dtop + dbottom);

    return childArea;
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
