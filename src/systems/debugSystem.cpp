#include "systems/debugSystem.hpp"

#include "components/components.hpp"
#include "resources/mesh.hpp"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

void DebugSystem::init() {
    cameraEntity = registry.view<CameraComponent>().front();
    debugEntity = registry.create();

    axisGeo = new Geometry(VertexAttributes{
                               VertexAttribute{3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0}
    },
                           GL_POINTS);

    const static std::vector<float> vertices = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0};

    axisGeo->bufferData(vertices, {0, 1, 2});
    Mesh* axisMesh = new Mesh();
    axisMesh->geometries[""] = {std::make_pair(MaterialPtr(nullptr), GeometryPtr(axisGeo))};

    registry.emplace<MeshComponent>(debugEntity, MeshPtr(axisMesh));
    registry.emplace<DebugComponent>(debugEntity);
    registry.emplace<TransformationComponent>(debugEntity, glm::vec3{0}, glm::quat(), glm::vec3{1}).calculateTransform();
}

DebugSystem::DebugSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<KeyEvent>().connect<&DebugSystem::handleKeyEvent>(*this);
}

DebugSystem::~DebugSystem() {
    delete axisGeo;
}

void DebugSystem::update(float dt) {
}

void DebugSystem::handleKeyEvent(const KeyEvent& e) {
    DebugComponent& debugComponent = registry.get<DebugComponent>(debugEntity);

    if (e.key == GLFW_KEY_F3 && e.action == GLFW_PRESS) {
        debugComponent.mode = debugComponent.mode == DebugMode::AXIS ? DebugMode::OFF : DebugMode::AXIS;
    }
    else if (e.key == GLFW_KEY_F4 && e.action == GLFW_PRESS) {
        debugComponent.mode = debugComponent.mode == DebugMode::SHADOW_MAPS ? DebugMode::OFF : DebugMode::SHADOW_MAPS;
    }

    game->debugMode = debugComponent.mode == DebugMode::OFF ? false : true;
}
