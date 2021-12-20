#pragma once
#include "guiElement.hpp"

#include <string>

class Label : public GuiElement {
  public:
    std::string text;
    Label(Gui* gui, const glm::vec4& backgroundColor, const std::string& text);

    virtual void render() const override;
};
