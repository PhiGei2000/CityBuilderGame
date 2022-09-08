#pragma once
#include "gui/components/widget.hpp"

#include "rendering/renderQuad.hpp"
#include "rendering/shader.hpp"
#include "rendering/textRenderer.hpp"

#include <stack>

class Application;

class PauseMenu;
class OptionsMenu;
class BuildMenu;

struct KeyEvent;
struct MouseButtonEvent;
struct MouseMoveEvent;

enum class GameMenus {
  NONE,
  MAIN_MENU,
  PAUSE_MENU,
  OPTIONS_MENU
};

class Gui {
  private:
    Application* app;

    RenderQuad quad;
    float width, height;

    PauseMenu* pauseMenu;
    OptionsMenu* optionsMenu;

    BuildMenu* buildMenu;

    std::stack<Widget*> navigation;    

    Shader* guiShader = new Shader("res/shaders/renderQuad.vert", "res/shaders/renderQuad.frag");

    void init();

  public:
    Gui(Application* app);

    TextRenderer textRenderer;

    void showMenu(GameMenus gameMenu);
    void popMenu();

    Application* getApp() const;
    Shader* getShader() const;
    const RenderQuad& getRenderQuad() const;

    void setScreenSize(float width, float height);
    Rectangle getBox() const;

    void render() const;

    void handleMouseButtonEvent(const MouseButtonEvent& e);
    void handleKeyEvent(const KeyEvent& e);
    void handleMouseMoveEvent(const MouseMoveEvent& e);
};
