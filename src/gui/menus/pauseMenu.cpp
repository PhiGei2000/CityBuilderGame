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
#include "gui/menus/pauseMenu.hpp"

#include "gui/components/button.hpp"
#include "gui/gui.hpp"

#include "application.hpp"

PauseMenu::PauseMenu(Gui* gui)
    : StackPanel("game_menu", gui, StackOrientation::COLUMN, colors::transparent) {

    constraints.width = RelativeConstraint(0.6f);
    constraints.height = AbsoluteConstraint(195.0f);

    TextButton* _continue = new TextButton("mainMenu_continue", gui, colors::anthraziteGrey, "Back to game");
    _continue->constraints.height = AbsoluteConstraint(45.0f);
    _continue->constraints.width = RelativeConstraint(0.9f);
    _continue->onClick += [&](const MouseButtonEvent& e) {
        this->onResumeButtonClick(e);
    };
    _continue->cornerRadius = 15.0f;
    addChild(_continue);

    TextButton* options = new TextButton("mainMenu_options", gui, colors::anthraziteGrey, "Options");
    options->constraints.height = AbsoluteConstraint(45.0f);
    options->constraints.width = RelativeConstraint(0.9f);
    options->onClick += [&](const MouseButtonEvent& e) {
        this->onOptionsButtonClick(e);
    };
    options->cornerRadius = 15.0f;
    addChild(options);

    TextButton* saveAndExit = new TextButton("mainMenu_saveExit", gui, colors::anthraziteGrey, "Save and Exit");
    saveAndExit->constraints.height = AbsoluteConstraint(45.0f);
    saveAndExit->constraints.width = RelativeConstraint(0.9f);
    saveAndExit->onClick += [&](const MouseButtonEvent& e) {
        this->onExitButtonClick(e);
    };
    saveAndExit->cornerRadius = 15.0f;
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
