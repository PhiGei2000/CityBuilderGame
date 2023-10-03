#include "gui/menus/buildMenu.hpp"

#include "application.hpp"
#include "events/buildEvent.hpp"
#include "events/mouseEvents.hpp"
#include "gui/components/button.hpp"
#include "gui/gui.hpp"

BuildMenu::BuildMenu(Gui* gui)
    : StackPanel("build_menu", gui, StackOrientation::COLUMN, colors::anthraziteGrey) {

    constraints.width = AbsoluteConstraint(128);
    constraints.x = RelativeConstraint(0.0f);
    cornerRadius = 0;
    itemAligment = ItemAligment::BEGIN;

    streetButtonTexture = new Texture("res/gui/streetBuilder_icon.png");
    streetButton = new IconButton("build_menu.button_street", gui, colors::anthraziteGrey, streetButtonTexture);
    streetButton->constraints.width = AbsoluteConstraint(64);
    streetButton->constraints.height = AbsoluteConstraint(64);
    streetButton->onClick += [&](MouseButtonEvent& e) {
        this->selectBuildingType(BuildingType::ROAD);

        e.handled = true;
    };
    addChild(streetButton);

    parkingLotButtonTexture = new Texture("res/gui/parking_lot_icon.png");
    parkingLotButton = new IconButton("build_menu.button_parkingLot", gui, colors::anthraziteGrey, parkingLotButtonTexture);
    parkingLotButton->constraints.width = AbsoluteConstraint(64);
    parkingLotButton->constraints.height = AbsoluteConstraint(64);
    parkingLotButton->onClick += [&](MouseButtonEvent& e) {
        this->selectBuildingType(BuildingType::PARKING_LOT);

        e.handled = true;
    };
    addChild(parkingLotButton);
}

void BuildMenu::selectBuildingType(BuildingType type) const {
    Game* game = this->gui->getApp()->getGame();

    BuildEvent event = BuildEvent{{}, type, BuildAction::SELECT};
    game->raiseEvent<BuildEvent>(event);
}
