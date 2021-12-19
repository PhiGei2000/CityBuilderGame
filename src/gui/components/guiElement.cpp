#include "gui/components/guiElement.hpp"

#include "gui/gui.hpp"

#include <iostream>

GuiElement::GuiElement(Gui* gui, const glm::vec4& backgroundColor)
    : gui{gui}, backgroundColor{backgroundColor} {
}

void GuiElement::render() const {
    if (!constraints.valid()) {
        return;
    }

    Rectangle area = getBox();

    // create transform
    glm::mat3 transform = glm::mat3(1.0f);
    transform[0].x = area.width / gui->width;
    transform[1].y = area.height / gui->height;
    transform[2].x = area.x / gui->width - 0.5f;
    transform[2].y = area.y / gui->height - 0.5f;

    Shader* guiShader = gui->quad.getShader();
    guiShader->use();

    guiShader->setMatrix3("transform", transform);
    guiShader->setVector4("color", backgroundColor);

    gui->quad.draw();
}

Rectangle GuiElement::getBox() const {
    Rectangle parentBox;
    if (parent == nullptr) {
        parentBox = Rectangle{gui->width / 2, gui->height / 2, gui->width, gui->height};
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

    // set center coordinates
    float x = parentBox.x, y = parentBox.y;
    switch (constraints.x.type) {
    case ConstraintType::ABSOLUTE:
        x += constraints.x.value;
        break;
    case ConstraintType::RELATIVE:
        x += constraints.x.value * parentBox.width;
        break;
    }

    switch (constraints.y.type) {
    case ConstraintType::ABSOLUTE:
        y += constraints.y.value;
        break;
    case ConstraintType::RELATIVE:
        y += constraints.y.value * parentBox.height;
    }

    return Rectangle{x, y, width, height};
}
