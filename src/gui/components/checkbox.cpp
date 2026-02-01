#include "gui/components/checkbox.hpp"

#include "gui/gui.hpp"

Checkbox::Checkbox(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, bool initialState, FontPtr font, const int textSize, TextAlign align, const glm::vec4& textColor)
    : Widget(id, gui, backgroundColor),
      Label(id, gui, backgroundColor, text, font, textSize, align, textColor), state(initialState) {
}

void Checkbox::applyConstraints() {
    Widget::applyConstraints();

    float checkboxAreaSize = box.height * 0.75;
    checkboxArea = Rectangle(box.getPosition() + 0.125f * glm::vec2(checkboxAreaSize), glm::vec2(checkboxAreaSize));

    updateTextGeometry(glm::vec2(box.height, 0.0f));

    if (constraints.height.getType() != ConstraintType::FIT_TO_CONTENT && constraints.width.getType() != ConstraintType::FIT_TO_CONTENT) {
        return;
    }

    if (constraints.height.getType() == ConstraintType::FIT_TO_CONTENT) {
        box.height = textHeight;
    }

    if (constraints.width.getType() == ConstraintType::FIT_TO_CONTENT) {
        box.width = textWidth;
    }
}

bool Checkbox::getState() const {
    return state;
}

void Checkbox::render() {
    if (!visible) {
        return;
    }

    Label::render();

    const RenderQuad& quad = gui->getRenderQuad();
    GuiShader* guiShader = gui->getShader();

    guiShader->setRectangle("widgetArea", checkboxArea);
    if (state) {
        guiShader->setVector4("color", checkboxColor);
    }
    else {
        guiShader->setVector4("color", colors::transparent);
        guiShader->setFloat("borderThickness", 3.0);
        guiShader->setVector4("borderColor", checkboxColor);
    }

    quad.draw(checkboxArea);
    guiShader->setFloat("borderThickness", 0.0);
}

void Checkbox::handleMouseButtonEvent(MouseButtonEvent& e) {
    if (e.action != GLFW_RELEASE || e.button != GLFW_MOUSE_BUTTON_LEFT) {
        return;
    }

    if (checkboxArea.pointInside(e.x, e.y)) {
        state = !state;

        CheckboxStateChangedEvent e;
        e.newState = state;
        onStateChanged.invoke(e);
    }
}