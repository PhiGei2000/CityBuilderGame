#include "gui/gui.hpp"

#include "gui/components/guiElement.hpp"
#include "gui/components/label.hpp"
#include "gui/components/stackPanel.hpp"

#include "events/keyEvent.hpp"
#include "events/mouseEvents.hpp"

#include "application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

Gui::Gui(Application* app)
    : app(app) {
    init();
}

void Gui::init() {
    StackPanel* mainMenuPanel = new StackPanel("mainMenu", this, StackPanel::StackOrientation::COLUMN, colors::transparent);
    mainMenuPanel->constraints.width = RelativeConstraint(0.6);
    mainMenuPanel->constraints.height = RelativeConstraint(0.5);

    Label* _continue = new Label("mainMenu_continue", this, colors::anthraziteGrey, "Back to game");
    _continue->constraints.x = CenterConstraint();
    _continue->constraints.height = RelativeConstraint(0.5f);
    _continue->constraints.width = RelativeConstraint(0.9f);
    mainMenuPanel->addChild(_continue);

    Label* saveAndExit = new Label("mainMenu_saveExit", this, colors::anthraziteGrey, "Save and Exit");
    saveAndExit->constraints.x = CenterConstraint();
    saveAndExit->constraints.height = RelativeConstraint(0.5f);
    saveAndExit->constraints.width = RelativeConstraint(0.9f);
    mainMenuPanel->addChild(saveAndExit);

    mainMenu = mainMenuPanel;

    textRenderer.init();
}

Shader* Gui::getShader() const {
    return guiShader;
}

void Gui::setScreenSize(float width, float height) {
    this->width = width;
    this->height = height;

    textRenderer.setScreenSize(width, height);
}

void Gui::render() const {
    guiShader->use();

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glm::mat4 projection = glm::ortho(0.0f, width, 0.0f, height);
    guiShader->setMatrix4("projection", projection);

    if (mainMenuVisible) {
        mainMenu->render();
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

const GuiElement* Gui::getElement(float x, float y) const {
    return mainMenu->getElementAt(x, height - y);
}

void Gui::handleMouseButtonEvent(const MouseButtonEvent& e) {
    if (e.action != GLFW_PRESS) {
        return;
    }

    const GuiElement* element = getElement(e.x, e.y);

    if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
        if (mainMenuVisible) {
            if (element->id == "mainMenu_saveExit") {
                // stop the application
                app->stop();
            }
            else if (element->id == "mainMenu_continue") {
                // close gui
                mainMenuVisible = false;
                app->resumeGame();
                app->setMouseVisibility(false);
            }
        }
    }
}

void Gui::handleKeyEvent(const KeyEvent& e) {
    if (e.action == GLFW_PRESS) {
        switch (e.key) {
        case GLFW_KEY_ESCAPE:
            mainMenuVisible = true;
            app->pauseGame();
            app->setMouseVisibility(true);
            break;
        }
    }
}
