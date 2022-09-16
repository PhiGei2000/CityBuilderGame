#include "gui/menus/buildMenu.hpp"

#include "application.hpp"
#include "events/buildEvent.hpp"
#include "gui/components/button.hpp"
#include "gui/gui.hpp"

BuildMenu::BuildMenu(Gui* gui)
    : StackPanel("build_menu", gui, StackOrientation::ROW, colors::transparent) {

    constraints.height = AbsoluteConstraint(0.1f);
    constraints.y = RelativeConstraint(0.9f);

    streetButtonTexture = new Texture("res/gui/streetBuilder_icon.png");
    streetButton = new IconButton("build_menu.button_street", gui, colors::anthraziteGrey, streetButtonTexture);
    streetButton->constraints.width = AbsoluteConstraint(64);
    streetButton->constraints.height = AbsoluteConstraint(64);
    streetButton->onClick += [&](const MouseButtonEvent& e) {
        this->selectBuildingType(BuildingType::ROAD);
    };

    addChild(streetButton);
}

void BuildMenu::selectBuildingType(BuildingType type) const {
    Game* game = this->gui->getApp()->getGame();

    BuildEvent event = BuildEvent{glm::ivec2(0), type, BuildAction::SELECT};
    game->raiseEvent<BuildEvent>(event);
}
