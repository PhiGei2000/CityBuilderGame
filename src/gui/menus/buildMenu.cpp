#include "gui/menus/buildMenu.hpp"

#include "application.hpp"
#include "events/buildEvent.hpp"
#include "events/mouseEvents.hpp"
#include "gui/components/button.hpp"
#include "gui/gui.hpp"
#include "resources/object.hpp"
#include "resources/roadPack.hpp"

#include <cctype>

BuildMenu::BuildMenu(Gui* gui)
    : Menu<StackPanel>("build_menu", gui, StackOrientation::COLUMN, colors::anthraziteGrey, ItemAligment::BEGIN) {
    this->spacing = 0.0f;

    constraints.x = CenterConstraint();
    constraints.y = CenterConstraint();
    constraints.width = RelativeConstraint(0.75f);
    constraints.height = RelativeConstraint(0.75f);

    title = new Label("build_menu.title", gui, colors::darkGrey, "Build menu", nullptr);
    title->constraints.x = AbsoluteConstraint();
    title->constraints.y = AbsoluteConstraint();
    title->constraints.height = AbsoluteConstraint(48.0f);
    addChild(title);

    for (unsigned int i = 0; i < static_cast<unsigned int>(BuildingCategory::BUILDING_CATEGORY_COUNT); i++) {
        BuildingCategory category = static_cast<BuildingCategory>(i);
        std::string categoryName = to_string(category);

        categoryName[0] = std::toupper(categoryName[0]);

        Label* categoryLabel = new Label(std::format("build_menu.label_{}", categoryName), gui, colors::darkGrey, categoryName, nullptr, 12);
        categoryLabel->constraints.height = AbsoluteConstraint(18.0f);
        addChild(categoryLabel);

        StackPanel* categoryStackPanel = new StackPanel(std::format("build_menu.stackPanel_{}", categoryName), gui, StackOrientation::ROW, colors::transparent, ItemAligment::BEGIN);
        categoryStackPanel->constraints.height = AbsoluteConstraint(84.0f);
        categoryStackPanel->padding.setHorizontal(AbsoluteConstraint(10.0f));
        categoryStackPanel->padding.setVertical(AbsoluteConstraint(10.0f));

        addChild(categoryStackPanel);

        categories[category] = std::make_pair(categoryLabel, categoryStackPanel);
    }

    const ResourceManager& resourceManager = gui->getApp()->getGame()->getResourceManager();
    for (const auto& [roadPackID, roadPack] : resourceManager.getResources<RoadPack>()) {
        addBuildingEntry(BuildMenuEntry{roadPack->name, "roads", BuildingCategory::INFRASTRUCTURE, roadPackID, roadPack->icon});
    }

    for (const auto& [buildingID, building] : resourceManager.getResources<BuildableObject>()) {
        addBuildingEntry(building->buildMenuEntry);
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

    button->constraints.y = AbsoluteConstraint(10.0f);
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
    game->raiseEvent<false>(event);
}
