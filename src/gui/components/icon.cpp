#include "gui/components/icon.hpp"

#include "gui/gui.hpp"

#include <GL/glew.h>

Icon::Icon(const std::string& id, Gui* gui, Texture* texture, const glm::vec4& backgroundColor)
    : GuiElement(id, gui, backgroundColor), texture(texture) {
}

void Icon::render() const {
    GuiElement::render();

    glActiveTexture(GL_TEXTURE0);

    const Rectangle& box = getBox();

    Shader* guiShader = gui->getShader();
    guiShader->setInt("tex", 0);
    guiShader->setBool("useTexture", true);

    texture->use(0);

    gui->getRenderQuad().draw(box.x, box.y, box.width, box.height);

    guiShader->setBool("useTexture", false);
}