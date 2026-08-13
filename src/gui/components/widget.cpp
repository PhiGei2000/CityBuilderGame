#include "gui/components/widget.hpp"

#include "gui/gui.hpp"

#include "events/mouseEvents.hpp"

#include <iostream>

Widget::Widget(const std::string& id, Gui* gui, const glm::vec4& backgroundColor)
    : id{id}, gui{gui}, backgroundColor{backgroundColor} {
}

void Widget::handleMouseButtonEvent(MouseButtonEvent& event) {
}

void Widget::handleMouseMoveEvent(MouseMoveEvent& event) {
    if (!visible)
        return;

    const Rectangle& area = getBox();

    // mouse enter
    if (area.pointInside(event.lastX, event.lastY)) {
        if (area.pointInside(event.x, event.y)) {
            onMouseEnter.invoke(event);
        }
    }

    // mouse leave
    if (!area.pointInside(event.x, event.y)) {
        if (area.pointInside(event.lastX, event.lastY)) {
            onMouseLeave.invoke(event);
        }
    }
}

void Widget::show() {
    visible = true;
}

void Widget::hide() {
    visible = false;
}

bool Widget::isVisible() const {
    return visible;
}

void Widget::update() {
}

void Widget::render() {
    if (!visible) {
        return;
    }

    if (!constraints.valid()) {
        return;
    }

    Rectangle area = getBox();

    GuiShader* guiShader = gui->getShader();
    guiShader->setVector4("color", backgroundColor);
    guiShader->setRectangle("widgetArea", area);
    guiShader->setFloat("cornerRadius", cornerRadius);

    const RenderQuad& quad = gui->getRenderQuad();
    quad.draw(area);
}

void Widget::applyConstraints() {
    Rectangle parentBox;
    if (parent == nullptr) {
        // friend class Widget
        parentBox = gui->getBox();
    }
    else {
        // TODO: Split functions to getPos, getWidth and getHeight
        parentBox = parent->getBox();
    }

    // set width and height values
    switch (constraints.height.getType()) {
        case ConstraintType::ABSOLUTE:
            box.height = constraints.height.getValue<AbsoluteConstraint>().value;
            break;
        case ConstraintType::RELATIVE:
            box.height = constraints.height.getValue<RelativeConstraint>().value * parentBox.height;
            break;
        case ConstraintType::FLEX:
            box.height = constraints.height.getValue<FlexConstraint>().absoluteValue;
            break;
        default:
            break;
    }

    switch (constraints.width.getType()) {
        case ConstraintType::ABSOLUTE:
            box.width = constraints.width.getValue<AbsoluteConstraint>().value;
            break;
        case ConstraintType::RELATIVE:
            box.width = constraints.width.getValue<RelativeConstraint>().value * parentBox.width;
            break;
        case ConstraintType::FLEX:
            box.width = constraints.width.getValue<FlexConstraint>().absoluteValue;
            break;
        default:
            break;
    }

    if (constraints.height.getType() == ConstraintType::ASPECT) {
        box.height = box.width / constraints.height.getValue<AspectConstraint>().value;
    }
    else if (constraints.width.getType() == ConstraintType::ASPECT) {
        box.width = box.height * constraints.width.getValue<AspectConstraint>().value;
    }

    // set coordinates of top left corner
    box.x = parentBox.x;
    box.y = parentBox.y;
    switch (constraints.x.getType()) {
        case ConstraintType::ABSOLUTE:
            box.x += constraints.x.getValue<AbsoluteConstraint>().value;
            break;
        case ConstraintType::RELATIVE:
            box.x += constraints.x.getValue<RelativeConstraint>().value * parentBox.width;
            break;
        case ConstraintType::CENTER:
            box.x += (parentBox.width - box.width) * 0.5f;
            break;
        default:
            break;
    }

    switch (constraints.y.getType()) {
        case ConstraintType::ABSOLUTE:
            box.y += constraints.y.getValue<AbsoluteConstraint>().value;
            break;
        case ConstraintType::RELATIVE:
            box.y += constraints.y.getValue<RelativeConstraint>().value * parentBox.height;
            break;
        case ConstraintType::CENTER:
            box.y += (parentBox.height - box.height) * 0.5f;
            break;
        default:
            break;
    }

    invalid = false;
}
