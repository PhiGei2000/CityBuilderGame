#include "gui/menus/gamePauseMenu.hpp"

#include "gui/components/button.hpp"
#include "gui/gui.hpp"

#include "application.hpp"

GamePauseMenu::GamePauseMenu(Gui* gui) 
    : StackPanel("game_menu", gui, StackOrientation::COLUMN, colors::transparent){
        
    constraints.width = RelativeConstraint(0.6);
    constraints.height = RelativeConstraint(0.5);

    Button* _continue = new Button("mainMenu_continue", gui, colors::anthraziteGrey, "Back to game");
    _continue->constraints.x = CenterConstraint();
    _continue->constraints.height = RelativeConstraint(0.5f);
    _continue->constraints.width = RelativeConstraint(0.9f);
    _continue->onClick += [&](const MouseButtonEvent& e) {
        this->onResumeButtonClick(e);
    };
    addChild(_continue);

    Button* options = new Button("mainMenu_options", gui, colors::anthraziteGrey, "Options");
    options->constraints.x = CenterConstraint();
    options->constraints.height = RelativeConstraint(0.5f);
    options->constraints.width = RelativeConstraint(0.9f);
    addChild(options);

    Button* saveAndExit = new Button("mainMenu_saveExit", gui, colors::anthraziteGrey, "Save and Exit");
    saveAndExit->constraints.x = CenterConstraint();
    saveAndExit->constraints.height = RelativeConstraint(0.5f);
    saveAndExit->constraints.width = RelativeConstraint(0.9f);
    addChild(saveAndExit);
}

void GamePauseMenu::onResumeButtonClick(const MouseButtonEvent& event) {
    hide();

    gui->getApp()->setGameState(GameState::RUNNING);
}
