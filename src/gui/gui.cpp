#include "gui/gui.hpp"

#include "gui/components/guiElement.hpp"
#include "gui/components/stackPanel.hpp"

void Gui::init() {
    StackPanel* mainMenuPanel = new StackPanel(this, StackPanel::StackOrientation::COLUMN, colors::transparent);
    mainMenuPanel->constraints.width = RelativeConstraint(0.6);
    mainMenuPanel->constraints.height = RelativeConstraint(0.5);

    for (int i = 0; i < 4; i++) {
        GuiElement* child = new GuiElement(this, colors::anthraziteGrey);

        child->constraints.x = AbsoluteConstraint(0);
        child->constraints.width = RelativeConstraint(0.9);
        child->constraints.height = RelativeConstraint(0.2f);

        mainMenuPanel->addChild(child);
    }

    mainMenu = mainMenuPanel;
}

void Gui::setScreenSize(float width, float height) {
    this->width = width;
    this->height = height;
}

void Gui::render() const {
    if (visible) {
        mainMenu->render();
    }
}
