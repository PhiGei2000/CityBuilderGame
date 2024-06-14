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
#include "gui/menus/debugPanel.hpp"

#include "gui/components/button.hpp"
#include "gui/gui.hpp"

#include "application.hpp"

#include "components/components.hpp"
#include "misc/utility.hpp"

DebugPanel::DebugPanel(Gui* gui)
    : StackPanel("debug_menu", gui, StackOrientation::COLUMN, colors::anthraziteGrey, ItemAligment::BEGIN) {
    constraints.x = AbsoluteConstraint(0);
    constraints.y = AbsoluteConstraint(0);
    constraints.width = RelativeConstraint(0.3);
    cornerRadius = 0.0f;

    TextButton* reloadResourcesButton = new TextButton("debug_menu.reloadResourcesButton", gui, colors::anthraziteGrey, "Reload Resources");
    reloadResourcesButton->constraints.height = AbsoluteConstraint(30);
    reloadResourcesButton->constraints.width = RelativeConstraint(0.9);
    reloadResourcesButton->onClick += [&](const MouseButtonEvent& e) {
        Application* app = this->gui->getApp();
        app->getGame()->reloadResources();
    };
    addChild(reloadResourcesButton);

    Label* fpsCounter = new Label("debug_menu.fpsCounter", gui, colors::transparent, "FPS: ");
    fpsCounter->textAlign = TextAlign::BEGIN;
    fpsCounter->constraints.height = AbsoluteConstraint(30);
    fpsCounter->constraints.width = RelativeConstraint(0.9);
    addChild(fpsCounter);

    Label* sunDirection = new Label("debug_menu.sunDirection", gui, colors::transparent, "");
    sunDirection->textAlign = TextAlign::BEGIN;
    sunDirection->constraints.height = AbsoluteConstraint(30);
    sunDirection->constraints.width = RelativeConstraint(0.9);
    addChild(sunDirection);

    Label* sunAngle = new Label("debug_menu.sunAngle", gui, colors::transparent, "");
    sunAngle->textAlign = TextAlign::BEGIN;
    sunAngle->constraints.height = AbsoluteConstraint(30);
    sunAngle->constraints.width = RelativeConstraint(0.9);
    addChild(sunAngle);

    Label* cameraPos = new Label("debug_menu.cameraPosition", gui, colors::transparent, "");
    cameraPos->textAlign = TextAlign::BEGIN;
    cameraPos->constraints.height = AbsoluteConstraint(30);
    cameraPos->constraints.width = RelativeConstraint(0.9);
    addChild(cameraPos);
}

void DebugPanel::update() {
    const Application* app = gui->getApp();
    Game* game = app->getGame();
    const entt::registry& registry = game->getRegistry();

    // fps
    float fps = 1.0f / app->updateTime;

    Label* fpsCounter = dynamic_cast<Label*>(getChild("debug_menu.fpsCounter"));
    fpsCounter->text = "FPS: " + std::to_string(fps);

    // sun info
    const SunLightComponent& sunLight = registry.get<SunLightComponent>(game->sun);
    const TransformationComponent& sunTransform = registry.get<TransformationComponent>(game->sun);

    Label* sunDirection = dynamic_cast<Label*>(getChild("debug_menu.sunDirection"));
    sunDirection->text = "Sun direction: (" + std::to_string(sunLight.direction) + ")";
    Label* sunAngle = dynamic_cast<Label*>(getChild("debug_menu.sunAngle"));
    sunAngle->text = "Sun angle: " + std::to_string(glm::degrees(sunLight.angle));

    // camera info
    const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(game->camera);

    Label* cameraPosition = dynamic_cast<Label*>(getChild("debug_menu.cameraPosition"));
    cameraPosition->text = "Camera position: (" + std::to_string(cameraTransform.position) + ")";
}
