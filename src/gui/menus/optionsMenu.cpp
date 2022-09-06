#include "gui/menus/optionsMenu.hpp"

#include "gui/components/label.hpp"

OptionsMenu::OptionsMenu(Gui* gui)
    : StackPanel("options_menu", gui, StackOrientation::COLUMN, colors::transparent) {

    Label* back = new Label("options_menu.back", gui, colors::anthraziteGrey, "Back");
    back->constraints.x = CenterConstraint();
    back->constraints.height = RelativeConstraint(0.5f);
    back->constraints.width = RelativeConstraint(0.9f);

    addChild(back);
}
