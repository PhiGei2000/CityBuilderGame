#pragma once
#include "gui/components/guiElement.hpp"
#include "renderQuad.hpp"
#include "colors.hpp"

class Gui {
  private:
    friend class GuiElement;

    RenderQuad quad;
    float width, height;

    GuiElement* mainMenu;

  public:
    void init();

    bool visible;

    void setScreenSize(float width, float height);

    void render() const;
};
