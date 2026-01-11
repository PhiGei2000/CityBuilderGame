#include "gui/menus/debugPanel.hpp"

#include "gui/components/button.hpp"
#include "gui/gui.hpp"

#include "application.hpp"

#include "components/components.hpp"
#include "misc/utility.hpp"

DebugPanel::DebugPanel(Gui* gui)
    : Menu<StackPanel>("debug_menu", gui, StackOrientation::COLUMN, colors::anthraziteGrey, ItemAligment::BEGIN) {
    constraints.x = AbsoluteConstraint(0);
    constraints.y = AbsoluteConstraint(0);
    constraints.width = RelativeConstraint(0.3);
    constraints.height = RelativeConstraint(1.0);
    cornerRadius = 0.0f;

    // TextButton* reloadResourcesButton = new TextButton("debug_menu.reloadResourcesButton", gui, colors::anthraziteGrey, "Reload Resources");
    // reloadResourcesButton->constraints.height = AbsoluteConstraint(30);
    // reloadResourcesButton->constraints.width = RelativeConstraint(0.9);
    // reloadResourcesButton->textSize = 12;
    // reloadResourcesButton->onClick += [&](const MouseButtonEvent& e) {
    //     Application* app = this->gui->getApp();
    //     app->getGame()->reloadResources();
    // };
    // addChild(reloadResourcesButton);

    Label* fpsCounter = new Label("debug_menu.fpsCounter", gui, colors::transparent, "FPS: ");
    fpsCounter->constraints.height = FitToContentConstraint();
    fpsCounter->constraints.width = RelativeConstraint(0.9);
    fpsCounter->textSize = 12;
    addChild(fpsCounter);

    Label* sunDirection = new Label("debug_menu.sunDirection", gui, colors::transparent, "");
    sunDirection->constraints.height = FitToContentConstraint();
    sunDirection->constraints.width = RelativeConstraint(0.9);
    sunDirection->textSize = 12;
    addChild(sunDirection);

    Label* sunAngle = new Label("debug_menu.sunAngle", gui, colors::transparent, "");
    sunAngle->constraints.height = FitToContentConstraint();
    sunAngle->constraints.width = RelativeConstraint(0.9);
    sunAngle->textSize = 12;
    addChild(sunAngle);

    Label* cameraPos = new Label("debug_menu.cameraPosition", gui, colors::transparent, "");
    cameraPos->constraints.height = FitToContentConstraint();
    cameraPos->constraints.width = RelativeConstraint(0.9);
    cameraPos->textSize = 12;
    addChild(cameraPos);

    TextButton* terrainShadingModeButton = new TextButton("debug_menu.terrainShadingModeButton", gui, colors::anthraziteGrey, "Terrain Shading");
    terrainShadingModeButton->constraints.height = AbsoluteConstraint(30);
    terrainShadingModeButton->constraints.width = RelativeConstraint(0.9);
    terrainShadingModeButton->textSize = 12;
    terrainShadingModeButton->onClick += [&](const MouseButtonEvent& e) {
        Application* app = this->gui->getApp();
        app->getGame()->terrain.shadingMode = app->getGame()->terrain.shadingMode == MeshShadingMode::SOLID ? MeshShadingMode::WIREFRAME : MeshShadingMode::SOLID;
    };
    addChild(terrainShadingModeButton);
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

    StackPanel::update();
}
