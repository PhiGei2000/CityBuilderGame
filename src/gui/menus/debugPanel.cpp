#include "gui/menus/debugPanel.hpp"

#include "gui/components/button.hpp"
#include "gui/gui.hpp"

#include "application.hpp"

DebugPanel::DebugPanel(Gui* gui)
    : StackPanel("debug_menu", gui, StackOrientation::COLUMN_REVERSE, colors::anthraziteGrey, ItemAligment::BEGIN) {
    constraints.x = AbsoluteConstraint(0);
    constraints.y = AbsoluteConstraint(0);
    constraints.width = RelativeConstraint(0.3);
    cornerRadius = 0.0f;

    Label* fpsCounter = new Label("debug_menu.fpsCounter", gui, colors::transparent, "FPS: ");
    fpsCounter->textAlign = TextAlign::BEGIN;
    fpsCounter->constraints.height = AbsoluteConstraint(30);
    fpsCounter->constraints.width = RelativeConstraint(0.9);    
    addChild(fpsCounter);

    TextButton* reloadResourcesButton = new TextButton("debug_menu.reloadResourcesButton", gui, colors::anthraziteGrey, "Reload Resources");
    reloadResourcesButton->constraints.height = AbsoluteConstraint(30);
    reloadResourcesButton->constraints.width = RelativeConstraint(0.9);    
    reloadResourcesButton->onClick += [&](const MouseButtonEvent& e) {
        Application* app = this->gui->getApp();
        app->getGame()->reloadResources();
    };
    addChild(reloadResourcesButton);
}