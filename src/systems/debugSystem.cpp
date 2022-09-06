#include "systems/debugSystem.hpp"

#include "components/components.hpp"

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

    axisGeo->fillBuffers(vertices, {0, 1, 2});

    registry.emplace<MultiMeshComponent>(debugEntity, std::unordered_map<std::string, MeshComponent>{
                                                          std::make_pair<std::string, MeshComponent>("axis", MeshComponent{std::shared_ptr<Geometry>(axisGeo),
                                                                                                                           resourceManager.getResource<Shader>("AXIS_SHADER"),
                                                                                                                           std::shared_ptr<Material>(nullptr)})});
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
    if (e.key == GLFW_KEY_F3 && e.action == GLFW_PRESS) {
        game->debugMode = !game->debugMode;
    }
}
