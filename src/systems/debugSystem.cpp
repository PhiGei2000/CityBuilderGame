#include "systems/debugSystem.hpp"

#include "components/components.hpp"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

void DebugSystem::init() {
    Shader* axisShader = new Shader("res/shaders/axis.vert", "res/shaders/axis.frag", "res/shaders/axis.geo");

    cameraEntity = registry.view<CameraComponent>().front();
    debugEntity = registry.create();

    Geometry* axisGeo = new Geometry(VertexAttributes{
                                         VertexAttribute{3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0}},
                                     GL_POINTS);

    const static std::vector<float> vertices = {
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0};

    axisGeo->fillBuffers(vertices, {0, 1, 2});

    registry.emplace<MultiMeshComponent>(debugEntity, std::unordered_map<std::string, Mesh>{
                                                          std::make_pair<std::string, Mesh>("axis", Mesh{std::shared_ptr<Geometry>(axisGeo),
                                                                                                         std::shared_ptr<Shader>(axisShader),
                                                                                                         std::shared_ptr<Texture>(nullptr)})/*,
                                                        std::make_pair<std::string, Mesh>("streets", Mesh{std::shared_ptr<Geometry>(nullptr),
                                                                                                            std::shared_ptr<Shader>(new Shader("res/shaders/streetDebug.vert", "res/shaders/streetDebug.frag")),
                                                                                                            std::shared_ptr<Texture>(nullptr)})*/});
    registry.emplace<DebugComponent>(debugEntity);
    registry.emplace<TransformationComponent>(debugEntity, glm::vec3{0}, glm::quat(), glm::vec3{1}).calculateTransform();
}

DebugSystem::DebugSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<KeyEvent>().connect<&DebugSystem::handleKeyEvent>(*this);
}

void DebugSystem::update(float dt) {
}

void DebugSystem::handleKeyEvent(const KeyEvent& e) {
    if (e.key == GLFW_KEY_F3 && e.action == GLFW_PRESS) {
        game->debugMode = !game->debugMode;
    }
}
