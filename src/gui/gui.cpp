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
}

void Gui::addMenu(int id, MenuBase* menu) {
    menus[id] = menu;

    widgets.push_back(menu->getWidget());
}

void Gui::showMenu(int menuID) {
    // hide the menu at the top of the navigation stack
    if (!navigation.empty())
        navigation.top()->hide();

    // switch (menu) {
    //     case GameMenus::NONE:
    //         // clear the navigation and set game state to running
    //         while (!navigation.empty()) {
    //             navigation.pop();
    //         }
    //         app->setGameState(GameState::RUNNING);
    //         return;
    //     case GameMenus::PAUSE_MENU:
    //         navigation.push(pauseMenu);
    //         break;
    //     case GameMenus::OPTIONS_MENU:
    //         navigation.push(optionsMenu);
    //         break;
    //     default:
    //         return;
    // }
    auto it = menus.find(menuID);
    if (it == menus.end()) {
        while (!navigation.empty()) {
            navigation.pop();
        }
        app->setGameState(GameState::RUNNING);
        return;
    }

    navigation.push(it->second->getWidget());

    // show the top menu on the navigation stack and set the game state to paused
    navigation.top()->show();
    app->setGameState(GameState::PAUSED);
}

void Gui::hideMenu(int menuID) {
    if (navigation.empty())
        return;

    auto it = menus.find(menuID);
    if (it != menus.end()) {
        if (navigation.top() != it->second->getWidget()) {
            return;
        }
    }

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

ShaderProgram* Gui::getShader() const {
    return guiShader;
}

const RenderQuad& Gui::getRenderQuad() const {
    return quad;
}

void Gui::setScreenSize(float width, float height) {
    // set new screen size and update the text renderer screen size
    this->width = width;
    this->height = height;

    // update widgets
    for (const auto& widget : widgets) {
        Container* container;
        if ((container = dynamic_cast<Container*>(widget)) != nullptr) {
            container->setChildConstraints();
        }
    }
}

void Gui::getScreenSize(float* width, float* height) const {
    *width = this->width;
    *height = this->height;
}

Rectangle Gui::getBox() const {
    return Rectangle{0, 0, width, height};
}

void Gui::init() {
    //     menus = {
    //         std::make_pair(static_cast<int>(GameMenus::OPTIONS_MENU), new OptionsMenu(this)),
    //         std::make_pair(static_cast<int>(GameMenus::PAUSE_MENU), new PauseMenu(this)),
    //         std::make_pair(static_cast<int>(GameMenus::BUILD_MENU), new BuildMenu(this)),
    // #if DEBUG
    //         std::make_pair(static_cast<int>(GameMenus::DEBUG_PANEL), new DebugPanel(this)),
    // #endif
    //     };

    //     for (const auto& [_, menu] : menus) {
    //         widgets.push_back(menu->getWidget());
    //     }

    warningWidget = new Label("warning_label", this, colors::transparent, "", nullptr, 12, TextAlign::BEGIN, colors::warning);
    warningWidget->hide();
}

void Gui::update() {
    for (Widget* widget : widgets) {
        if (widget->isVisible()) {
            widget->update();
        }
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

    for (const auto [_, menu] : menus) {
        menu->getWidget()->render();
    }

    warningWidget->render();

    // enable depth test and disable blend
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void Gui::handleMouseButtonEvent(MouseButtonEvent& event) {
    if (!navigation.empty()) {
        navigation.top()->handleMouseButtonEvent(event);
    }
}

void Gui::handleKeyEvent(KeyEvent& e) {
    if (e.action != GLFW_PRESS) {
        return;
    }

    if (e.key == GLFW_KEY_ESCAPE) {
        hideMenu(0);
    }

    if (app->getGameState() == GameState::RUNNING) {
        for (auto it = menus.begin(); it != menus.end(); it++) {
            MenuBase* menu = it->second;
            if (menu->getKey() == e.key) {
                if (menu->toggleOnKey()) {
                    if (menu->getWidget()->isVisible()) {
                        hideMenu(it->first);
                    }
                    else {
                        showMenu(it->first);
                    }
                }
                else {
                    showMenu(it->first);
                    e.handled = true;
                }
                break;
            }
        }
    }
}

void Gui::handleMouseMoveEvent(MouseMoveEvent& event) {

    if (!navigation.empty()) {
        navigation.top()->handleMouseMoveEvent(event);
    }

    for (const auto& [_, menu] : menus) {
        menu->getWidget()->handleMouseMoveEvent(event);
    }
}
