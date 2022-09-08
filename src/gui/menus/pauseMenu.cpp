#include "gui/menus/pauseMenu.hpp"

#include "gui/components/button.hpp"
#include "gui/gui.hpp"

#include "application.hpp"

PauseMenu::PauseMenu(Gui* gui) 
    : StackPanel("game_menu", gui, StackOrientation::COLUMN, colors::transparent){
        
    constraints.width = RelativeConstraint(0.6);
    constraints.height = RelativeConstraint(0.5);

    TextButton* _continue = new TextButton("mainMenu_continue", gui, colors::anthraziteGrey, "Back to game");
    _continue->constraints.x = CenterConstraint();
    _continue->constraints.height = RelativeConstraint(0.5f);
    _continue->constraints.width = RelativeConstraint(0.9f);
    _continue->onClick += [&](const MouseButtonEvent& e) {
        this->onResumeButtonClick(e);
    };
    addChild(_continue);

    TextButton* options = new TextButton("mainMenu_options", gui, colors::anthraziteGrey, "Options");
    options->constraints.x = CenterConstraint();
    options->constraints.height = RelativeConstraint(0.5f);
    options->constraints.width = RelativeConstraint(0.9f);
    options->onClick += [&](const MouseButtonEvent& e) {
        this->onOptionsButtonClick(e);
    };
    addChild(options);

    TextButton* saveAndExit = new TextButton("mainMenu_saveExit", gui, colors::anthraziteGrey, "Save and Exit");
    saveAndExit->constraints.x = CenterConstraint();
    saveAndExit->constraints.height = RelativeConstraint(0.5f);
    saveAndExit->constraints.width = RelativeConstraint(0.9f);
    saveAndExit->onClick += [&](const MouseButtonEvent& e){
        this->onExitButtonClick(e);
    };
    addChild(saveAndExit);
}

void PauseMenu::onResumeButtonClick(const MouseButtonEvent& event) {
    gui->showMenu(GameMenus::NONE);
}

void PauseMenu::onOptionsButtonClick(const MouseButtonEvent& event) {
    gui->showMenu(GameMenus::OPTIONS_MENU);
}

void PauseMenu::onExitButtonClick(const MouseButtonEvent& event) {
    // TODO: Add save progress
    
    gui->getApp()->stop();
}
