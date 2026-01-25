#include "gui/components/slider.hpp"

#include "gui/gui.hpp"
#include "rendering/shader.hpp"

#include "application.hpp"

Slider::Slider(const std::string& id, Gui* gui, const glm::vec4& color, const glm::vec4& backgroundColor)
    : Widget(id, gui, backgroundColor), color(color),
      sliderBarWidth(RelativeConstraint(0.9f)),
      sliderBarHeight(AbsoluteConstraint(10)),
      sliderIndicatorSize(AbsoluteConstraint(15)) {
}

void Slider::applyConstraints() {
    Widget::applyConstraints();

    // slider bar
    float sliderBarWidth = 0.0f;
    switch (this->sliderBarWidth.getType()) {
        case ConstraintType::ABSOLUTE:
            sliderBarWidth = this->sliderBarWidth.getValue<AbsoluteConstraint>().value;
            break;
        case ConstraintType::RELATIVE:
            sliderBarWidth = this->sliderBarWidth.getValue<RelativeConstraint>().value * box.width;
            break;
    }

    float sliderBarHeight = 0.0f;
    switch (this->sliderBarHeight.getType()) {
        case ConstraintType::ABSOLUTE:
            sliderBarHeight = this->sliderBarHeight.getValue<AbsoluteConstraint>().value;
            break;
        case ConstraintType::RELATIVE:
            sliderBarHeight = this->sliderBarHeight.getValue<RelativeConstraint>().value * box.height;
            break;
    }

    const glm::vec2 sliderBarPos = glm::vec2{
        box.x + (box.width - sliderBarWidth) / 2.0f,
        box.y + (box.height - sliderBarHeight) / 2.0f,
    };

    const glm::vec2 sliderBarSize = {sliderBarWidth, sliderBarHeight};
    sliderBarArea = Rectangle(sliderBarPos, sliderBarSize);

    indicatorArea = getIndicatorBox();
}

Rectangle Slider::getIndicatorBox() const {
    const glm::vec2& sliderBarPos = sliderBarArea.getPosition();

    const glm::vec2 indicatorSize(sliderIndicatorSize.value);
    const glm::vec2 indicatorPos = {
        sliderBarPos.x + value * sliderBarArea.width / (maxValue - minValue) - indicatorSize.x / 2.0f,
        sliderBarPos.y + (sliderBarArea.height - indicatorSize.y) / 2.0f,
    };

    return Rectangle(indicatorPos, indicatorSize);
}

void Slider::render() {
    Widget::render();

    GuiShader* shader = gui->getShader();
    shader->setVector4("color", color);
    shader->setRectangle("widgetArea", sliderBarArea);
    shader->setFloat("cornerRadius", 0.0f);

    const RenderQuad& quad = gui->getRenderQuad();
    quad.draw(sliderBarArea);

    shader->setRectangle("widgetArea", indicatorArea);
    shader->setFloat("cornerRadius", indicatorArea.height / 2.0f);

    quad.draw(indicatorArea);
}

void Slider::handleMouseButtonEvent(MouseButtonEvent& e) {
    if (!visible) {
        return;
    }

    if (e.button != GLFW_MOUSE_BUTTON_LEFT) {
        return;
    }

    const Rectangle& box = getIndicatorBox();
    if (!box.pointInside(e.x, e.y) && e.action == GLFW_PRESS) {
        return;
    }

    mouseDown = (e.action == GLFW_PRESS);
}

void Slider::handleMouseMoveEvent(MouseMoveEvent& e) {
    if (!visible) {
        return;
    }

    // check if mouse is pressed
    if (!mouseDown) {
        return;
    }

    const auto [minX, maxX] = sliderBarArea.getX();
    float x = glm::clamp(e.x, minX, maxX);

    value = (x - minX) * (maxValue - minValue) / sliderBarArea.width + minValue;
    invalid = true;
}
