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

void Widget::render() const {
    if (!visible) {
        return;
    }

    if (!constraints.valid()) {
        return;
    }

    Rectangle area = getBox();

    Shader* guiShader = gui->getShader();
    guiShader->setVector4("color", backgroundColor);
    guiShader->setVector2("widgetArea.position", glm::vec2{area.x, area.y});
    guiShader->setVector2("widgetArea.size", glm::vec2{area.width, area.height});
    guiShader->setFloat("cornerRadius", cornerRadius);

    const RenderQuad& quad = gui->getRenderQuad();
    quad.draw(area.x, area.y, area.width, area.height);
}

Rectangle Widget::getBox() const {
    Rectangle parentBox;
    if (parent == nullptr) {
        // friend class Widget
        parentBox = gui->getBox();
    }
    else {
        parentBox = parent->getBox();
    }

    // set width and height values
    float width, height;
    switch (constraints.height.type) {
        case ConstraintType::ABSOLUTE:
            height = constraints.height.value;
            break;
        case ConstraintType::RELATIVE:
            height = constraints.height.value * parentBox.height;
            break;
    }

    switch (constraints.width.type) {
        case ConstraintType::ABSOLUTE:
            width = constraints.width.value;
            break;
        case ConstraintType::RELATIVE:
            width = constraints.width.value * parentBox.width;
            break;
    }

    if (constraints.height.type == ConstraintType::ASPECT) {
        height = width / constraints.height.value;
    }
    else if (constraints.width.type == ConstraintType::ASPECT) {
        width = height * constraints.width.value;
    }

    // set coordinates of top left corner
    float x = parentBox.x;
    float y = parentBox.y;
    switch (constraints.x.type) {
        case ConstraintType::ABSOLUTE:
            x += constraints.x.value;
            break;
        case ConstraintType::RELATIVE:
            x += constraints.x.value * parentBox.width;
            break;
        case ConstraintType::CENTER:
            x += (parentBox.width - width) * 0.5f;
            break;
    }

    switch (constraints.y.type) {
        case ConstraintType::ABSOLUTE:
            y += constraints.y.value;
            break;
        case ConstraintType::RELATIVE:
            y += constraints.y.value * parentBox.height;
            break;
        case ConstraintType::CENTER:
            y += (parentBox.height - height) * 0.5f;
            break;
    }

    return Rectangle{x, y, width, height};
}