#pragma once
#include "colors.hpp"

#include "gui/components/guiElement.hpp"

#include "rendering/renderQuad.hpp"
#include "rendering/shader.hpp"
#include "rendering/textRenderer.hpp"

class Application;

struct KeyEvent;
struct MouseButtonEvent;

class Gui {
  private:
    friend class GuiElement;

    Application* app;

    RenderQuad quad;
    float width, height;

    GuiElement* mainMenu;
    bool mainMenuVisible = false;

    Shader* guiShader = new Shader("res/shaders/renderQuad.vert", "res/shaders/renderQuad.frag");

    void init();

  public:
    Gui(Application* app);

    TextRenderer textRenderer;

    Shader* getShader() const;

    void setScreenSize(float width, float height);

    void render() const;

    const GuiElement* getElement(float x, float y) const;

    void handleMouseButtonEvent(const MouseButtonEvent& e);
    void handleKeyEvent(const KeyEvent& e);
};
