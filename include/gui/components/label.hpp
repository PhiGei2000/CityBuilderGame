#pragma once
#include "../colors.hpp"
#include "widget.hpp"
#include "../colors.hpp"

#include "rendering/textRenderer.hpp"
#include <string>

class Label : public virtual Widget {
  public:
    std::string text;
    glm::vec4 textColor;

    TextAlign textAlign = TextAlign::CENTER;

    Label(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, const glm::vec4& textColor = colors::white);

    virtual void render() const override;
};
