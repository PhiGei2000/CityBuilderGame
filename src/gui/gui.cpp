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
#include "gui/gui.hpp"

#include "gui/components/icon.hpp"
#include "gui/components/label.hpp"
#include "gui/components/stackPanel.hpp"
#include "gui/components/widget.hpp"

#include "gui/menus/buildMenu.hpp"
#include "gui/menus/debugPanel.hpp"
#include "gui/menus/optionsMenu.hpp"
#include "gui/menus/pauseMenu.hpp"

#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

#include "rendering/texture.hpp"

#include "application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

Gui::Gui(Application* app, float width, float height)
    : app(app), width(width), height(height) {
    init();
}

void Gui::init() {
    textRenderer.init();

    pauseMenu = new PauseMenu(this);
    optionsMenu = new OptionsMenu(this);
    buildMenu = new BuildMenu(this);
    debugPanel = new DebugPanel(this);
    warningWidget = new Label("warning_label", this, colors::transparent, "", 12, colors::warning);
    warningWidget->hide();

    widgets = {pauseMenu, optionsMenu, buildMenu, debugPanel};
}

void Gui::showMenu(GameMenus menu) {
    // hide the menu at the top of the navigation stack
    if (!navigation.empty())
        navigation.top()->hide();

    switch (menu) {
        case GameMenus::NONE:
            // clear the navigation and set game state to running
            while (!navigation.empty()) {
                navigation.pop();
            }
            app->setGameState(GameState::RUNNING);
            return;
        case GameMenus::PAUSE_MENU:
            navigation.push(pauseMenu);
            break;
        case GameMenus::OPTIONS_MENU:
            navigation.push(optionsMenu);
            break;
        default:
            return;
    }

    // show the top menu on the navigation stack and set the game state to paused
    navigation.top()->show();
    app->setGameState(GameState::PAUSED);
}

void Gui::popMenu() {
    if (navigation.empty())
        return;

    // hide the top menu and remove it from the navigation stack
    navigation.top()->hide();
    navigation.pop();

    // if the navigation is not empty show the menu at the top
    if (!navigation.empty()) {
        navigation.top()->show();
    }
    else {
        // if the navigation stack is empty set the game state to running
        app->setGameState(GameState::RUNNING);
    }
}

void Gui::showWarning(const std::string& text) const {
    warningWidget->text = text;
    warningWidget->show();
}

void Gui::hideWarning() const {
    warningWidget->hide();
}

Application* Gui::getApp() const {
    return app;
}

Shader* Gui::getShader() const {
    return guiShader;
}

const RenderQuad& Gui::getRenderQuad() const {
    return quad;
}

void Gui::setScreenSize(float width, float height) {
    // set new screen size and update the text renderer screen size
    this->width = width;
    this->height = height;

    textRenderer.setScreenSize(width, height);

    // update widgets
    for (const auto& widget : widgets) {
        Container* container;
        if ((container = dynamic_cast<Container*>(widget)) != nullptr) {
            container->setChildConstraints();
        }
    }
}

Rectangle Gui::getBox() const {
    return Rectangle{0, 0, width, height};
}

void Gui::update() {
    for (Widget* widget : widgets) {
        widget->update();
    }
}

void Gui::render() const {
    // bind the shader
    guiShader->use();

    // disable depth test and enable blend
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // set default uniforms
    glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f);
    guiShader->setMatrix4("projection", projection);
    guiShader->setBool("text", false);
    guiShader->setBool("useTexture", false);
    guiShader->setInt("tex", 0);

    // render top menu
    if (!navigation.empty()) {
        navigation.top()->render();
    }

    if (app->getGameState() == GameState::BUILD_MODE) {
        buildMenu->show();
        buildMenu->render();
    }
    else {
        buildMenu->hide();
    }

    debugPanel->render();

    warningWidget->render();

    // enable depth test and disable blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void Gui::handleMouseButtonEvent(MouseButtonEvent& event) {
    if (!navigation.empty()) {
        navigation.top()->handleMouseButtonEvent(event);
    }

    if (debugPanel->isVisible()) {
        debugPanel->handleMouseButtonEvent(event);
    }

    if (buildMenu->isVisible()) {
        buildMenu->handleMouseButtonEvent(event);
    }
}

void Gui::handleKeyEvent(KeyEvent& e) {
    if (e.action == GLFW_PRESS) {
        if (app->getGameState() == GameState::RUNNING) {
            switch (e.key) {
                case GLFW_KEY_ESCAPE:
                    if (!navigation.empty()) {
                        popMenu();
                    }
                    else {
                        showMenu(GameMenus::PAUSE_MENU);
                    }

                    e.handled = true;
                    break;
                case GLFW_KEY_F1:
                    if (debugPanel->isVisible()) {
                        debugPanel->hide();
                    }
                    else {
                        debugPanel->show();
                    }
                    e.handled = true;
                    break;
            }
        }
        else if (app->getGameState() == GameState::PAUSED) {
            if (e.key == GLFW_KEY_ESCAPE) {
                popMenu();
            }
        }
    }
}

void Gui::handleMouseMoveEvent(MouseMoveEvent& event) {

    if (!navigation.empty()) {
        navigation.top()->handleMouseMoveEvent(event);
    }

    if (debugPanel->isVisible()) {
        debugPanel->handleMouseMoveEvent(event);
    }
}
