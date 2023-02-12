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
class DebugPanel;
class Label;

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
    DebugPanel* debugPanel;
  Label* warningWidget;

    std::stack<Widget*> navigation;
    std::vector<Widget*> widgets;

    Shader* guiShader = new Shader("res/shaders/renderQuad.vert", "res/shaders/renderQuad.frag");

    void init();

  public:
    Gui(Application* app, float width, float height);

    TextRenderer textRenderer;

    void showMenu(GameMenus gameMenu);
    void popMenu();

    void showWarning(const std::string& text) const;
    void hideWarning() const;

    Application* getApp() const;
    Shader* getShader() const;
    const RenderQuad& getRenderQuad() const;

    void setScreenSize(float width, float height);
    Rectangle getBox() const;

    void update();

    void render() const;

    void handleMouseButtonEvent(MouseButtonEvent& e);
    void handleKeyEvent(KeyEvent& e);
    void handleMouseMoveEvent(MouseMoveEvent& e);
};
