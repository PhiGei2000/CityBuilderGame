#include "gui/menus/optionsMenu.hpp"

#include "gui/components/button.hpp"
#include "gui/gui.hpp"

OptionsMenu::OptionsMenu(Gui* gui)
    : StackPanel("options_menu", gui, StackOrientation::COLUMN, colors::transparent) {

        
    StackPanel* row = new StackPanel("options_menu.last_row", gui, StackOrientation::ROW, colors::transparent);

    TextButton* back = new TextButton("options_menu.back", gui, colors::anthraziteGrey, "Back");
    back->constraints.x = CenterConstraint();
    back->constraints.height = RelativeConstraint(0.5f);
    back->constraints.width = RelativeConstraint(0.9f);
    back->onClick += [&](const MouseButtonEvent& e) {
        this->gui->popMenu();
    };
    row->addChild(back);

    TextButton* done = new TextButton("options_menu.done", gui, colors::anthraziteGrey, "Done");
    done->constraints.x = CenterConstraint();
    done->constraints.height = RelativeConstraint(0.5f);
    done->constraints.width = RelativeConstraint(0.9f);
    done->onClick += [&](const MouseButtonEvent& e) {
        this->gui->popMenu();
    };
    row->addChild(done);

    addChild(row);
}
