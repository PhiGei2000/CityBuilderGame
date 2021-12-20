#pragma once
#include "../colors.hpp"
#include "guiElement.hpp"

#include <vector>

class ContainerItem : public GuiElement {
  protected:
    GuiElement* element;

  public:
    ContainerItem(const std::string& id, Gui* gui, GuiElement* element, const glm::vec4 backgroundColor = colors::transparent);

    void render() const override;
};

class Container : public GuiElement {
  protected:
    std::vector<ContainerItem*> children;

  public:
    Container(const std::string& id, Gui* gui, const glm::vec4& backgroundColor);
    virtual void addChild(GuiElement* child);

    void render() const override;

    virtual void setChildConstraints() = 0;
};
