#include "systems/debugSystem.hpp"

#include "components/components.hpp"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

void DebugSystem::init() {
    axisShader = new Shader("res/shaders/axis.vert", "res/shaders/axis.frag", "res/shaders/axis.geo");

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBufferData(GL_ARRAY_BUFFER, sizeof(axis), axis, GL_STATIC_DRAW);

    cameraEntity = registry.view<CameraComponent>().front();
}

DebugSystem::DebugSystem(Game* game)
    : System(game) {
    init();

    eventDispatcher.sink<KeyEvent>().connect<&DebugSystem::handleKeyEvent>(*this);
}

void DebugSystem::update(float dt) {
    if (game->debugMode) {
        axisShader->use();

        const CameraComponent& camera = registry.get<CameraComponent>(cameraEntity);
        const glm::vec3& cameraPos = registry.get<TransformationComponent>(cameraEntity).position;
        
        axisShader->setVector3("cameraTarget", cameraPos + 5.0f * camera.front);

        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, 3);
    }
}

void DebugSystem::handleKeyEvent(const KeyEvent& e) {
    if (e.key == GLFW_KEY_F3 && e.action == GLFW_PRESS) {
        game->debugMode = !game->debugMode;
    }
}
