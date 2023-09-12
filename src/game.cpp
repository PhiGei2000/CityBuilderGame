#include "game.hpp"

#include "application.hpp"
#include "components/components.hpp"
#include "events/events.hpp"
#include "misc/utility.hpp"
#include "misc/coordinateTransform.hpp"
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
    systems.push_back(new TerrainSystem(this));
    systems.push_back(new RoadSystem(this));
    systems.push_back(new CarSystem(this));
    systems.push_back(new EnvironmentSystem(this));
    systems.push_back(new PhysicsSystem(this));
    systems.push_back(new DebugSystem(this));
    systems.push_back(new RenderSystem(this));

    // entities

    sun = registry.create();
    constexpr float sunAngle = glm::radians(45.0f);
    registry.emplace<SunLightComponent>(sun,
                                        sunAngle,        // direction
                                        glm::vec3(0.5f), // ambient
                                        glm::vec3(0.9f), // diffuse
                                        glm::vec3(0.8f)  // specular
    );
    registry.emplace<TransformationComponent>(sun, utility::sphericalToCartesian(300.0f, sunAngle, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));

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

// handle framebuffer size and camera events even if the game is paused
template<>
void Game::raiseEvent<FramebufferSizeEvent>(FramebufferSizeEvent& e) {
    eventDispatcher.trigger<FramebufferSizeEvent&>(e);
}

template<>
void Game::raiseEvent<CameraUpdateEvent>(CameraUpdateEvent& e) {
    eventDispatcher.trigger<CameraUpdateEvent&>(e);
}

template<typename Event>
void Game::raiseEvent(Event& e) {
    if (state != GameState::PAUSED) {
        eventDispatcher.trigger<Event&>(e);
    }
}

template<>
void Game::raiseEvent(BuildEvent& e) {
    if (state != GameState::PAUSED) {
        eventDispatcher.trigger<BuildEvent&>(e);

        if (!e.valid) {
            app->getGui()->showWarning("Invalid build position!");
        }
        else {
            app->getGui()->hideWarning();
        }
    }
}

template void Game::raiseEvent<FramebufferSizeEvent>(FramebufferSizeEvent&);
template void Game::raiseEvent<KeyEvent>(KeyEvent&);
template void Game::raiseEvent<MouseButtonEvent>(MouseButtonEvent&);
template void Game::raiseEvent<MouseMoveEvent>(MouseMoveEvent&);
template void Game::raiseEvent<MouseScrollEvent>(MouseScrollEvent&);
template void Game::raiseEvent<CameraUpdateEvent>(CameraUpdateEvent&);
template void Game::raiseEvent<ChunkCreatedEvent>(ChunkCreatedEvent&);
template void Game::raiseEvent<ChunkDestroyedEvent>(ChunkDestroyedEvent&);
