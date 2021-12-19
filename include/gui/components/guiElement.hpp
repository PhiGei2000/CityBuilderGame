#pragma once
#include "../constraints.hpp"
#include "../rectangle.hpp"

#include <glm/glm.hpp>

class Gui;

class GuiElement {
  protected:
    Gui* gui;

  public:
    GuiElement(Gui* gui, const glm::vec4& backgroundColor);

    GuiElement* parent = nullptr;
    glm::vec4 backgroundColor;
    Constraints constraints;

    virtual void render() const;    

    Rectangle getBox() const;
};
