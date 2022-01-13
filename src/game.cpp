#include "game.hpp"

#include "application.hpp"
#include "components/components.hpp"
#include "events/events.hpp"
#include "modelLoader.hpp"
#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "systems/systems.hpp"

Game::Game(Application* app)
    : app(app) {
    loadResources();

    init();
}

void Game::loadResources() {
    // models
    resourceManager.setResource<Geometry>("GROUND_GEOMETRY", new Geometry(ModelLoader::load("res/models/ground.obj")));
    resourceManager.setResource<Geometry>("TREE_GEOMETRY", new Geometry(ModelLoader::load("res/models/tree.obj")));
    resourceManager.setResource<Geometry>("BUILDMARKER_GEOMETRY", new Geometry(ModelLoader::load("res/models/buildMarker.obj")));

    // resourceManager.setResource<Geometry>("STREET_STRAIGHT_GEOMETRY", ModelLoader::load("res/models/street_straight.obj"));
    // resourceManager.setResource<Geometry>("STREET_CURVE_GEOMETRY", ModelLoader::load("res/models/street_curve.obj"));
    // resourceManager.setResource<Geometry>("STREET_NOT_CONNECTED_GEOMETRY", ModelLoader::load("res/models/street_notConnected.obj"));
    // resourceManager.setResource<Geometry>("STREET_END_GEOMETRY", ModelLoader::load("res/models/street_end.obj"));
    // resourceManager.setResource<Geometry>("STREET_T_CROSSING_GEOMETRY", ModelLoader::load("res/models/street_t_crossing.obj"));
    // resourceManager.setResource<Geometry>("STREET_CROSSING_GEOMETRY", ModelLoader::load("res/models/street_crossing.obj"));

    // shaders
    resourceManager.setResource<Shader>("MESH_SHADER", new Shader("res/shaders/mesh.vert", "res/shaders/mesh.frag"));

    // textures
    resourceManager.setResource<Texture>("GROUND_TEXTURE", new Texture("res/textures/ground.png"));
    resourceManager.setResource<Texture>("STREET_TEXTURE", new Texture("res/textures/street.png"));
    resourceManager.setResource<Texture>("TREE_TEXTURE", new Texture("res/textures/tree.png"));
    resourceManager.setResource<Texture>("BUILDMARKER_TEXTURE", new Texture("res/textures/buildMarker.png", GL_RGBA));
}

void Game::init() {
    // init systems
    systems.push_back(new CameraSystem(this));
    systems.push_back(new BuildSystem(this));
    systems.push_back(new StreetSystem(this));
    systems.push_back(new EnvironmentSystem(this));
    systems.push_back(new PhysicsSystem(this));
    systems.push_back(new RenderSystem(this));

    // entities
    entt::entity groundEntity = registry.create();
    registry.emplace<MeshComponent>(groundEntity,
                                    resourceManager.getResource<Geometry>("GROUND_GEOMETRY"),
                                    resourceManager.getResource<Shader>("MESH_SHADER"),
                                    resourceManager.getResource<Texture>("GROUND_TEXTURE"));

    registry.emplace<TransformationComponent>(groundEntity, glm::vec3(0.0f, 0.0f, 0.0f), glm::quat(), glm::vec3(1.0f, 1.0f, 1.0f));
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
void Game::raiseEvent(const Event& e) {
    if (state != GameState::PAUSED) {
        eventDispatcher.trigger<Event>(e);
    }
}

template void Game::raiseEvent<FramebufferSizeEvent>(const FramebufferSizeEvent&);
template void Game::raiseEvent<KeyEvent>(const KeyEvent&);
template void Game::raiseEvent<MouseButtonEvent>(const MouseButtonEvent&);
template void Game::raiseEvent<MouseMoveEvent>(const MouseMoveEvent&);
template void Game::raiseEvent<BuildEvent>(const BuildEvent&);