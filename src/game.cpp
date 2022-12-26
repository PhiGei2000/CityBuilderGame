#include "game.hpp"

#include "application.hpp"
#include "components/components.hpp"
#include "events/events.hpp"
#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "systems/systems.hpp"

Game::Game(Application* app)
    : app(app), resourceManager("res/") {

    init();
}

void Game::init() {
    // init systems
    systems.push_back(new CameraSystem(this));
    camera = registry.view<CameraComponent>().front();

    systems.push_back(new BuildSystem(this));
    systems.push_back(new RoadSystem(this));
    systems.push_back(new CarSystem(this));
    systems.push_back(new EnvironmentSystem(this));
    systems.push_back(new PhysicsSystem(this));
    systems.push_back(new DebugSystem(this));
    systems.push_back(new RenderSystem(this));

    // entities
    entt::entity groundEntity = registry.create();

    MeshComponent& mesh = registry.emplace<MeshComponent>(groundEntity, resourceManager.getResource<Mesh>("GROUND_MESH"));

    registry.emplace<TransformationComponent>(groundEntity, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));

    sun = registry.create();
    registry.emplace<LightComponent>(sun,
                                     glm::vec3{0, -1, 0}, // direction
                                     glm::vec3(0.5f),     // ambient
                                     glm::vec3(0.9f),     // diffuse
                                     glm::vec3(0.8f)      // specular
    );
    registry.emplace<TransformationComponent>(sun, 300.0f * glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));

    EntityMoveEvent moveEvent{sun};
    raiseEvent(moveEvent);
}

entt::registry& Game::getRegistry() {
    return registry;
}

entt::dispatcher& Game::getEventDispatcher() {
    return eventDispatcher;
}

ResourceManager& Game::getResourceManager() {
    return resourceManager;
}

void Game::update(float dt) {
    if (state == GameState::PAUSED) {
        // update render system
        systems.back()->update(dt);
    }
    else {
        for (System* system : systems) {
            system->update(dt);
        }
    }
}

void Game::reloadResources() {
    resourceManager.loadResources();
}

int Game::getKey(int key) const {
    return glfwGetKey(app->getWindow(), key);
}

glm::vec2 Game::getMousePos() const {
    double x, y;
    int width, height;
    glfwGetCursorPos(app->getWindow(), &x, &y);
    glfwGetFramebufferSize(app->getWindow(), &width, &height);

    return glm::vec2((float)(2 * x) / width - 1.0f, 1.0f - (float)(2 * y) / height);
}

void Game::setState(GameState state) {
    this->state = state;
}

GameState Game::getState() const {
    return state;
}

template<typename Event>
void Game::raiseEvent(Event& e) {
    if (state != GameState::PAUSED) {
        eventDispatcher.trigger<Event&>(e);
    }
}

template void Game::raiseEvent<FramebufferSizeEvent>(FramebufferSizeEvent&);
template void Game::raiseEvent<KeyEvent>(KeyEvent&);
template void Game::raiseEvent<MouseButtonEvent>(MouseButtonEvent&);
template void Game::raiseEvent<MouseMoveEvent>(MouseMoveEvent&);
template void Game::raiseEvent<BuildEvent>(BuildEvent&);
template void Game::raiseEvent<CameraUpdateEvent>(CameraUpdateEvent&);
