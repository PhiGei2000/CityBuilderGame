#pragma once
#include "widget.hpp"

#include <string>

class Label : public Widget {
  public:
    std::string text;
    Label(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text);

    virtual void render() const override;
};
