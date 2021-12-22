#include "gui/components/label.hpp"
#include "gui/gui.hpp"

Label::Label(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text)
    : GuiElement(id, gui, backgroundColor), text(text) {
}

void Label::render() const {
    GuiElement::render();

    const Rectangle& box = getBox();
    Shader* guiShader = gui->getShader();

    guiShader->setBool("flipV", true);
    guiShader->setBool("text", true);
    guiShader->setVector4("color", colors::white);

    gui->textRenderer.renderText(text, box, 0.5f);

    guiShader->setBool("flipV", false);
    guiShader->setBool("text", false);
}
