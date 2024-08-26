/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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

    ShaderProgram* guiShader = new ShaderProgram("res/shaders/renderQuad.vert", "res/shaders/renderQuad.frag");

    void init();

  public:
    Gui(Application* app, float width, float height);

    TextRenderer textRenderer;

    void showMenu(GameMenus gameMenu);
    void popMenu();

    void showWarning(const std::string& text) const;
    void hideWarning() const;

    Application* getApp() const;
    ShaderProgram* getShader() const;
    const RenderQuad& getRenderQuad() const;
    BuildMenu* getBuildMenu() const;

    void setScreenSize(float width, float height);
    Rectangle getBox() const;

    void update();

    void render() const;

    void handleMouseButtonEvent(MouseButtonEvent& e);
    void handleKeyEvent(KeyEvent& e);
    void handleMouseMoveEvent(MouseMoveEvent& e);
};
