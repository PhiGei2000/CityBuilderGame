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
#include "gui/menus/buildMenu.hpp"

#include "application.hpp"
#include "events/buildEvent.hpp"
#include "events/mouseEvents.hpp"
#include "gui/components/button.hpp"
#include "gui/gui.hpp"

#include <cctype>

BuildMenu::BuildMenu(Gui* gui)
    : StackPanel("build_menu", gui, StackOrientation::COLUMN, colors::anthraziteGrey, ItemAligment::BEGIN) {
    this->outerSpacing = false;
    this->spacing = 0.0f;

    constraints.x = CenterConstraint();
    constraints.y = CenterConstraint();
    constraints.width = RelativeConstraint(0.75f);
    constraints.height = RelativeConstraint(0.75f);

    title = new Label("build_menu.title", gui, colors::darkGrey, "Build menu");
    title->constraints.x = AbsoluteConstraint();
    title->constraints.y = AbsoluteConstraint();
    title->constraints.height = AbsoluteConstraint(32.0f);
    title->textAlign = TextAlign::BEGIN;
    addChild(title);

    for (unsigned int i = 0 ; i < static_cast<unsigned int>(BuildingCategory::BUILDING_CATEGORY_COUNT); i++) {
        BuildingCategory category = static_cast<BuildingCategory>(i);
        std::string categoryName = to_string(category);

        categoryName[0] = std::toupper(categoryName[0]);

        Label* categoryLabel = new Label(std::format("build_menu.label_{}", categoryName), gui, colors::darkGrey, categoryName, 12);
        categoryLabel->constraints.height = AbsoluteConstraint(18.0f);
        categoryLabel->textAlign = TextAlign::BEGIN;
        addChild(categoryLabel);

        StackPanel* categoryStackPanel = new StackPanel(std::format("build_menu.stackPanel_{}", categoryName), gui, StackOrientation::ROW, colors::transparent, ItemAligment::BEGIN);
        categoryStackPanel->constraints.height = AbsoluteConstraint(64.0f);
        addChild(categoryStackPanel);

        categories[category] = std::make_pair(categoryLabel, categoryStackPanel);
    }
}

BuildMenu::~BuildMenu() {
    for (const auto& [_, iconButton] : buttons) {
        delete iconButton;
    }

    for (const auto& [_, category] : categories) {
        delete category.first;
        delete category.second;
    }

    delete title;
}

void BuildMenu::addBuildingEntry(const BuildMenuEntry& entry) {
    Texture* iconTexture = new Texture(entry.iconFilename);

    IconButton* button = new IconButton(std::format("build_menu.button_{}", entry.buildingID), gui, colors::anthraziteGrey, iconTexture);

    button->constraints.width = AbsoluteConstraint(64);
    button->constraints.height = AbsoluteConstraint(64);
    button->onClick += [=, this](MouseButtonEvent& e) {
        this->selectBuildingType(entry.buildingID);

        e.handled = true;
        this->hide();
    };

    StackPanel* panel = categories.at(entry.category).second;
    panel->addChild(button);
}

void BuildMenu::selectBuildingType(const std::string& buildingID) const {
    Game* game = this->gui->getApp()->getGame();

    BuildingSelectedEvent event = BuildingSelectedEvent(buildingID);
    game->raiseEvent<BuildingSelectedEvent>(event);
}
