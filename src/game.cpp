#include "game.hpp"

#include "application.hpp"
#include "components/components.hpp"
#include "events/events.hpp"
#include "gui/menus/buildMenu.hpp"
#include "misc/coordinateTransform.hpp"
#include "misc/utility.hpp"
#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "systems/systems.hpp"

Game::Game(Application* app)
    : app(app), resourceManager("res/", app), terrain(this) {
#if DEBUG
    logStream = std::ofstream("log.txt");
#endif

    init();
}

void Game::init() {
    // init camera system
    systems.push_back(new CameraSystem(this));
    // entities
    camera = registry.view<CameraComponent>().front();
    sun = registry.create();

    // init other systems
    systems.push_back(new BuildSystem(this));
    systems.push_back(new TerrainSystem(this));
    systems.push_back(new RoadSystem(this));
    systems.push_back(new CarSystem(this));
    systems.push_back(new EnvironmentSystem(this));
    systems.push_back(new PhysicsSystem(this));
    systems.push_back(new DebugSystem(this));
    systems.push_back(new RenderSystem(this));
}

entt::registry& Game::getRegistry() {
    return registry;
}

const entt::registry& Game::getRegistry() const {
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

int Game::getMouseButton(int button) const {
    return glfwGetMouseButton(app->getWindow(), button);
}

void Game::setState(GameState state) {
    GameStateChangedEvent e(this->state, state);
    this->state = state;

    raiseEvent<true>(e);

    // if (state == GameState::BUILD_MODE) {
    //     app->getGui()->getBuildMenu()->show();
    // }
    // else {
    //     app->getGui()->getBuildMenu()->hide();
    // }
}

GameState Game::getState() const {
    return state;
}

#if DEBUG
void Game::log(const std::string& message) {
    logStream << message << std::endl;
}
#endif
