#pragma once
#include "../constraints.hpp"
#include "../rectangle.hpp"

#include <string>

#include <glm/glm.hpp>

class Gui;

class GuiElement {
  protected:
    Gui* gui;

  public:
    const std::string id;

    GuiElement(const std::string& id, Gui* gui, const glm::vec4& backgroundColor);

    GuiElement* parent = nullptr;
    glm::vec4 backgroundColor;
    Constraints constraints;

    virtual void render() const;

    Rectangle getBox() const;
};
