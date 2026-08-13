#include "gui/menus/debugPanel.hpp"

#include "gui/components/button.hpp"
#include "gui/components/checkbox.hpp"
#include "gui/gui.hpp"

#include "application.hpp"

#include "components/components.hpp"
#include "misc/utility.hpp"

#include <fstream>

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

    Label* fpsCounter = new Label("debug_menu.fpsCounter", gui, colors::transparent, "FPS: ", nullptr, 12);
    fpsCounter->constraints.height = FitToContentConstraint();
    fpsCounter->constraints.width = RelativeConstraint(0.9);
    addChild(fpsCounter);

    Label* sunDirection = new Label("debug_menu.sunDirection", gui, colors::transparent, "", nullptr, 12);
    sunDirection->constraints.height = FitToContentConstraint();
    sunDirection->constraints.width = RelativeConstraint(0.9);
    addChild(sunDirection);

    Label* sunAngle = new Label("debug_menu.sunAngle", gui, colors::transparent, "", nullptr, 12);
    sunAngle->constraints.height = FitToContentConstraint();
    sunAngle->constraints.width = RelativeConstraint(0.9);
    addChild(sunAngle);

    Label* cameraPos = new Label("debug_menu.cameraPosition", gui, colors::transparent, "", nullptr, 12);
    cameraPos->constraints.height = FitToContentConstraint();
    cameraPos->constraints.width = RelativeConstraint(0.9);
    addChild(cameraPos);

    Checkbox* terrainShadingModeCheckbox = new Checkbox("debug_menu.terrainShadingModeCheckbox", gui, colors::transparent, "Wireframe Terrain Shading", false, nullptr, 12);
    terrainShadingModeCheckbox->constraints.height = AbsoluteConstraint(30);
    terrainShadingModeCheckbox->constraints.width = RelativeConstraint(0.9);
    terrainShadingModeCheckbox->onStateChanged += [&](const Checkbox::CheckboxStateChangedEvent& e) {
        Application* app = this->gui->getApp();
        app->getGame()->terrain.shadingMode = e.newState ? MeshShadingMode::WIREFRAME : MeshShadingMode::SOLID;
    };
    addChild(terrainShadingModeCheckbox);

    Checkbox* carPathCheckbox = new Checkbox("debug_menu.carPathCheckbox", gui, colors::transparent, "Draw car paths", false, nullptr, 12);
    carPathCheckbox->constraints.height = AbsoluteConstraint(30);
    carPathCheckbox->constraints.width = RelativeConstraint(0.9f);
    carPathCheckbox->onStateChanged += [&](const Checkbox::CheckboxStateChangedEvent& e) {
        Game* game = this->gui->getApp()->getGame();
        game->debugOptions.drawCarPaths = e.newState;
    };
    addChild(carPathCheckbox);

    TextButton* saveRoadGraph = new TextButton("debug_menu.saveRoadGraph", gui, colors::transparent, "Save road graph", nullptr, 12);
    saveRoadGraph->constraints.height = AbsoluteConstraint(30);
    saveRoadGraph->constraints.width = RelativeConstraint(0.9f);
    saveRoadGraph->onClick += [&](const MouseButtonEvent& e) {
        Game* game = this->gui->getApp()->getGame();
        auto activeChunks = game->terrain.chunkEntities;

        std::ofstream file;
        file.open("roadGraph.txt");

        for (auto [pos, chunkEntity] : activeChunks) {
            file << "chunk position: " << pos << "\r\n";
            const RoadGraph& graph = game->getRegistry().get<RoadComponent>(chunkEntity).graph;

            file << graph;
        }

        file.close();
    };
    addChild(saveRoadGraph);
}

void DebugPanel::update() {
    const Application* app = gui->getApp();
    Game* game = app->getGame();
    const entt::registry& registry = game->getRegistry();

    // fps
    float fps = 1.0f / app->updateTime;

    Label* fpsCounter = dynamic_cast<Label*>(getChild("debug_menu.fpsCounter"));
    // fpsCounter->text = "FPS: " + std::to_string(fps);
    fpsCounter->setText("FPS: " + std::to_string(fps));

    // sun info
    const SunLightComponent& sunLight = registry.get<SunLightComponent>(game->sun);
    const TransformationComponent& sunTransform = registry.get<TransformationComponent>(game->sun);

    Label* sunDirection = dynamic_cast<Label*>(getChild("debug_menu.sunDirection"));
    // sunDirection->text = "Sun direction: (" + std::to_string(sunLight.direction) + ")";
    sunDirection->setText("Sun direction: (" + std::to_string(sunLight.direction) + ")");
    Label* sunAngle = dynamic_cast<Label*>(getChild("debug_menu.sunAngle"));
    // sunAngle->text = "Sun angle: " + std::to_string(glm::degrees(sunLight.angle));
    sunAngle->setText("Sun angle: " + std::to_string(glm::degrees(sunLight.angle)));

    // camera info
    const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(game->camera);

    Label* cameraPosition = dynamic_cast<Label*>(getChild("debug_menu.cameraPosition"));
    // cameraPosition->text = "Camera position: (" + std::to_string(cameraTransform.position) + ")";
    cameraPosition->setText("Camera position: (" + std::to_string(cameraTransform.position) + ")");

    StackPanel::update();
}
