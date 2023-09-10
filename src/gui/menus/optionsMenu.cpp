#include "gui/menus/optionsMenu.hpp"

#include "gui/components/button.hpp"
#include "gui/gui.hpp"

OptionsMenu::OptionsMenu(Gui* gui)
    : StackPanel("options_menu", gui, StackOrientation::COLUMN, colors::transparent) {
    constraints.width = RelativeConstraint(0.6f);
    constraints.height = AbsoluteConstraint(120.0f);

    TextButton* test = new TextButton("options_menu.test", gui, colors::anthraziteGrey, "Test");
    test->constraints.height = AbsoluteConstraint(45.0f);
    test->constraints.width = RelativeConstraint(1.0f);
    addChild(test);

    StackPanel* row = new StackPanel("options_menu.last_row", gui, StackOrientation::ROW, colors::transparent);
    row->constraints.height = AbsoluteConstraint(45.0f);
    row->constraints.width = RelativeConstraint(1.0f);
    row->outerSpacing = false;
    addChild(row);

    TextButton* back = new TextButton("options_menu.back", gui, colors::anthraziteGrey, "Back");
    back->constraints.height = RelativeConstraint(1.0f);
    back->constraints.width = RelativeConstraint(0.5f);
    back->onClick += [&](const MouseButtonEvent& e) {
        this->gui->popMenu();
    };
    back->cornerRadius = 15.0f;
    row->addChild(back);

    TextButton* done = new TextButton("options_menu.done", gui, colors::anthraziteGrey, "Done");
    done->constraints.height = RelativeConstraint(1.0f);
    done->constraints.width = RelativeConstraint(0.5f);
    done->onClick += [&](const MouseButtonEvent& e) {
        this->gui->popMenu();
    };
    done->cornerRadius = 15.0f;
    row->addChild(done);

}
