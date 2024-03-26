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
#include <vector>

#include "game.hpp"
#include "gui/gui.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class System;
struct KeyEvent;
struct FramebufferSizeEvent;
struct MouseButtonEvent;
struct MouseMoveEvent;
struct MouseScrollEvent;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursorPos_callback(GLFWwindow* window, double x, double y);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

class Application {
  private:
    GLFWwindow* window;

    Gui* gui = nullptr;
    Game* game = nullptr;

    bool stopRequested = false;

    friend void cursorPos_callback(GLFWwindow*, double, double);
    glm::vec2 lastCursorPos = glm::vec2(400.0f, 300.0f);

    void init();

  public:
    float updateTime = 0.0f;

    Application();

    void run();

    void stop();

    inline Game* getGame() const {
        return game;
    }

    inline void setGameState(GameState state) {
        game->setState(state);
    }

    inline const Gui* getGui() const {
        return gui;
    }

    inline GameState getGameState() const {
        return game->getState();
    }

    GLFWwindow* getWindow() const;

    void onKeyEvent(KeyEvent& e);
    void onFramebufferSizeEvent(FramebufferSizeEvent& e);
    void onMouseMoveEvent(MouseMoveEvent& e);
    void onMouseButtonEvent(MouseButtonEvent& e);
    void onMouseScrollEvent(MouseScrollEvent& e);
};
