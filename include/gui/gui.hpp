#pragma once
#include "gui/components/widget.hpp"
#include "gui/menus/menu.hpp"

#include "rendering/renderQuad.hpp"
#include "rendering/shader.hpp"

#include <map>
#include <stack>

class Application;
class Label;

struct KeyEvent;
struct MouseButtonEvent;
struct MouseMoveEvent;
struct GameStateChangedEvent;

class Gui {
  private:
    Application* app;

    RenderQuad quad;
    float width, height;

    std::map<int, MenuBase*> menus;

    Label* warningWidget;

    std::stack<Widget*> navigation;
    std::vector<Widget*> widgets;

    ShaderProgram* guiShader = new ShaderProgram("res/shaders/renderQuad.vert", "res/shaders/renderQuad.frag");

  public:
    Gui(Application* app, float width, float height);

    void addMenu(int menuID, MenuBase* menu);
    void showMenu(int menuID);
    void hideMenu(int menuID);

    void showWarning(const std::string& text) const;
    void hideWarning() const;

    Application* getApp() const;
    ShaderProgram* getShader() const;
    const RenderQuad& getRenderQuad() const;

    void setScreenSize(float width, float height);
    void getScreenSize(float* width, float* height) const;
    Rectangle getBox() const;

    void init();
    void update();

    void render() const;

    void handleMouseButtonEvent(MouseButtonEvent& e);
    void handleKeyEvent(KeyEvent& e);
    void handleMouseMoveEvent(MouseMoveEvent& e);
};
