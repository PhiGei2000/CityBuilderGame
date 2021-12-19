#pragma once
#include "guiElement.hpp"

#include <vector>

class Container : public GuiElement {
  protected:
    std::vector<GuiElement*> children;

  public:
    Container(Gui* gui, const glm::vec4& backgroundColor);
    virtual void addChild(GuiElement* child);

    void render() const override;

    virtual void setChildConstraints() = 0;
};