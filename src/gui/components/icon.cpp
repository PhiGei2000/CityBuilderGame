#include "gui/components/icon.hpp"

Icon::Icon(const std::string& id, Gui* gui, Texture* texture, const glm::vec4& backgroundColor)
    : GuiElement(id, gui, backgroundColor), texture(texture) {
}

void Icon::render() const {
    
}