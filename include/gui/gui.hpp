#pragma once
#include "colors.hpp"
#include "gui/components/guiElement.hpp"
#include "rendering/renderQuad.hpp"
#include "rendering/shader.hpp"
#include "rendering/textRenderer.hpp"

class Gui {
  private:
    friend class GuiElement;

    RenderQuad quad;
    float width, height;

    GuiElement* mainMenu;
    Shader* guiShader = new Shader("res/shaders/renderQuad.vert", "res/shaders/renderQuad.frag");

  public:
    TextRenderer textRenderer;

    void init();

    bool visible;

    Shader* getShader() const;

    void setScreenSize(float width, float height);

    void render() const;

    const GuiElement* getElement(float x, float y) const;
};
