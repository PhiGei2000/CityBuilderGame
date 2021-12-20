#include "gui/components/guiElement.hpp"

#include "gui/gui.hpp"

#include <iostream>

GuiElement::GuiElement(const std::string& id, Gui* gui, const glm::vec4& backgroundColor)
    : id{id}, gui{gui}, backgroundColor{backgroundColor} {
}

void GuiElement::render() const {
    if (!constraints.valid()) {
        return;
    }

    Rectangle area = getBox();

    Shader* guiShader = gui->guiShader;
    guiShader->setVector4("color", backgroundColor);

    gui->quad.draw(area.x, area.y, area.width, area.height);
}

Rectangle GuiElement::getBox() const {
    Rectangle parentBox;
    if (parent == nullptr) {
        parentBox = Rectangle{0, 0, gui->width, gui->height};
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

    // set coordinates of lower left corner
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

const GuiElement* GuiElement::getElementAt(float x, float y) const {
    const Rectangle& area = getBox();
    if (area.pointInside(x, y)) {
        return this;
    }

    return nullptr;
}
