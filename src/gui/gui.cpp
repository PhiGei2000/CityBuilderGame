#include "gui/gui.hpp"

#include "gui/components/icon.hpp"
#include "gui/components/label.hpp"
#include "gui/components/stackPanel.hpp"
#include "gui/components/widget.hpp"

#include "gui/menus/optionsMenu.hpp"
#include "gui/menus/pauseMenu.hpp"

#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

#include "rendering/texture.hpp"

#include "application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

Gui::Gui(Application* app)
    : app(app) {
    init();
}

void Gui::init() {
    pauseMenu = new PauseMenu(this);
    optionsMenu = new OptionsMenu(this);    

    textRenderer.init();
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
}

Rectangle Gui::getBox() const {
    return Rectangle{0, 0, width, height};
}

void Gui::render() const {
    // bind the shader
    guiShader->use();

    // disable depth test and enable blend
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // set default uniforms
    glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height);
    guiShader->setMatrix4("projection", projection);
    guiShader->setBool("text", false);
    guiShader->setBool("useTexture", false);
    guiShader->setBool("flipV", false);
    guiShader->setInt("tex", 0);

    // render top menu
    if (!navigation.empty()) {
        navigation.top()->render();
    }

    // enable depth test and disable blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void Gui::handleMouseButtonEvent(const MouseButtonEvent& e) {
    MouseButtonEvent event(e);
    event.y = height - e.y;

    if (!navigation.empty()) {
        navigation.top()->handleMouseButtonEvent(event);
    }
}

void Gui::handleKeyEvent(const KeyEvent& e) {
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
                break;
            }
        }
    }
}

void Gui::handleMouseMoveEvent(const MouseMoveEvent& e) {
    MouseMoveEvent event(e);
    event.y = height - e.y;
    event.lastY = height - e.lastY;

    if (!navigation.empty()) {
        navigation.top()->handleMouseMoveEvent(event);
    }
}
