#pragma once
#include "guiElement.hpp"
#include "gui/colors.hpp"

#include "rendering/texture.hpp"

class Icon : public GuiElement {
  protected:
    Texture* texture;

  public:
    Icon(const std::string& id, Gui* gui, Texture* texture, const glm::vec4& backgroundColor = colors::transparent);

    void render() const override;
};