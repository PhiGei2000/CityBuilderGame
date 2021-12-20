#include "gui/gui.hpp"

#include "gui/components/guiElement.hpp"
#include "gui/components/label.hpp"
#include "gui/components/stackPanel.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

void Gui::init() {
    StackPanel* mainMenuPanel = new StackPanel(this, StackPanel::StackOrientation::COLUMN, colors::transparent);
    mainMenuPanel->constraints.width = RelativeConstraint(0.6);
    mainMenuPanel->constraints.height = RelativeConstraint(0.5);

    for (int i = 0; i < 3; i++) {
        GuiElement* child = new GuiElement(this, colors::anthraziteGrey);

        child->constraints.x = CenterConstraint();
        child->constraints.width = RelativeConstraint(0.9);
        child->constraints.height = RelativeConstraint(0.5);

        mainMenuPanel->addChild(child);
    }

    Label* label = new Label(this, colors::anthraziteGrey, "Save and Exit");    
    label->constraints.width = RelativeConstraint(0.9);
    label->constraints.height = RelativeConstraint(0.5);
    mainMenuPanel->addChild(label);

    mainMenu = mainMenuPanel;

    // mainMenu = new GuiElement(this, colors::anthraziteGrey);

    // mainMenu->constraints.x = CenterConstraint();
    // mainMenu->constraints.width = RelativeConstraint(0.5);
    // mainMenu->constraints.y = AbsoluteConstraint(100);
    // mainMenu->constraints.height = RelativeConstraint(0.5);

    textRenderer.init();
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

    if (visible) {
        mainMenu->render();
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}
