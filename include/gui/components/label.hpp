#pragma once
#include "widget.hpp"
#include "../colors.hpp"

#include <string>

class Label : public virtual Widget {
  public:
    std::string text;
    glm::vec4 textColor;

    Label(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, const glm::vec4& textColor = colors::white);

    virtual void render() const override;
};
