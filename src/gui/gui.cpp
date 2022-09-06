#include "gui/gui.hpp"

#include "gui/components/icon.hpp"
#include "gui/components/label.hpp"
#include "gui/components/stackPanel.hpp"
#include "gui/components/widget.hpp"

#include "gui/menus/gamePauseMenu.hpp"
#include "gui/menus/optionsMenu.hpp"

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
    gamePauseMenu = new GamePauseMenu(this);
    optionsMenu = new OptionsMenu(this);

    // StackPanel* toolboxPanel = new StackPanel("toolbox", this, StackPanel::StackOrientation::ROW, colors::transparent);
    // toolboxPanel->constraints.x = AbsoluteConstraint(50);
    // toolboxPanel->constraints.y = AbsoluteConstraint(50);
    // toolboxPanel->constraints.height = AbsoluteConstraint(48);

    // Texture* streetBuilderIconTexture = new Texture("res/gui/streetBuilder_icon.png", GL_RGBA);
    // Icon* streetBuilder = new Icon("toolbox_streetBuilder", this, streetBuilderIconTexture, colors::anthraziteGrey);
    // streetBuilder->constraints.x = AbsoluteConstraint(0);
    // streetBuilder->constraints.y = AbsoluteConstraint(0);
    // streetBuilder->constraints.height = AbsoluteConstraint(48);
    // streetBuilder->constraints.width = AspectConstraint(1);
    // toolboxPanel->addChild(streetBuilder);

    // toolbox = toolboxPanel;

    textRenderer.init();
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
    this->width = width;
    this->height = height;

    textRenderer.setScreenSize(width, height);
}

Rectangle Gui::getBox() const {
    return Rectangle{0, 0, width, height};
}

void Gui::render() const {
    guiShader->use();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    // set default uniforms
    glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height);
    guiShader->setMatrix4("projection", projection);
    guiShader->setBool("text", false);
    guiShader->setBool("useTexture", false);
    guiShader->setBool("flipV", false);
    guiShader->setInt("tex", 0);

    gamePauseMenu->render();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

const Widget* Gui::getElement(float x, float y) const {
    const Widget* element = nullptr;

    if (gamePauseMenu->isVisible()) {
        element = gamePauseMenu->getElementAt(x, height - y);
    }

    // if (element == nullptr && toolboxVisible) {
    //     element = toolbox->getElementAt(x, height - y);
    // }

    return element;
}

void Gui::handleMouseButtonEvent(const MouseButtonEvent& e) {
    MouseButtonEvent event(e);
    event.y = height - e.y;

    gamePauseMenu->handleMouseButtonEvent(event);
    optionsMenu->handleMouseButtonEvent(event);
}

void Gui::handleKeyEvent(const KeyEvent& e) {
    if (e.action == GLFW_PRESS) {
        switch (e.key) {
        case GLFW_KEY_ESCAPE:
            if (gamePauseMenu->isVisible()) {
                gamePauseMenu->hide();
                app->setGameState(GameState::RUNNING);
            }
            else {
                gamePauseMenu->show();
                app->setGameState(GameState::PAUSED);
            }
            break;
        }
    }
}

void Gui::handleMouseMoveEvent(const MouseMoveEvent& e) {
    MouseMoveEvent event(e);
    event.y = height - e.y;
    event.lastY = height - e.lastY;

    gamePauseMenu->handleMouseMoveEvent(event);
    optionsMenu->handleMouseMoveEvent(event);
}
