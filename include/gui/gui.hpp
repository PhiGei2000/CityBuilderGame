#pragma once
#include "gui/components/widget.hpp"

#include "rendering/renderQuad.hpp"
#include "rendering/shader.hpp"
#include "rendering/textRenderer.hpp"

class Application;

class GamePauseMenu;
class OptionsMenu;

struct KeyEvent;
struct MouseButtonEvent;
struct MouseMoveEvent;

class Gui {
  private:
    Application* app;

    RenderQuad quad;
    float width, height;

    GamePauseMenu* gamePauseMenu;
    OptionsMenu* optionsMenu;

    Shader* guiShader = new Shader("res/shaders/renderQuad.vert", "res/shaders/renderQuad.frag");

    void init();

  public:
    Gui(Application* app);

    TextRenderer textRenderer;

    Application* getApp() const;
    Shader* getShader() const;
    const RenderQuad& getRenderQuad() const;

    void setScreenSize(float width, float height);
    Rectangle getBox() const;

    void render() const;

    const Widget* getElement(float x, float y) const;

    void handleMouseButtonEvent(const MouseButtonEvent& e);
    void handleKeyEvent(const KeyEvent& e);
    void handleMouseMoveEvent(const MouseMoveEvent& e);
};
