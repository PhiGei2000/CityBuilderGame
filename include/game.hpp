#pragma once
#include "resources/resourceManager.hpp"

#include "misc/typedefs.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <fstream>

class System;
class Application;

enum class GameState {
    PAUSED,
    RUNNING,
    BUILD_MODE
};

class Game {
    std::vector<System*> systems;

    ResourceManager resourceManager;

    entt::registry registry;
    entt::dispatcher eventDispatcher;

    Application* app;

    void init();

    GameState state = GameState::RUNNING;

#if DEBUG
    std::ofstream logStream;
#endif

  public:
    entt::entity camera;
    entt::entity sun = entt::null;
    std::unordered_map<glm::ivec2, entt::entity> terrain;

    Game(Application* app);

    bool debugMode = false;

    entt::registry& getRegistry();
    entt::dispatcher& getEventDispatcher();
    ResourceManager& getResourceManager();

    void update(float dt);
    void reloadResources();

    int getKey(int key) const;

    glm::vec2 getMousePos() const;

    void setState(GameState state);
    GameState getState() const;

    template<typename Event>
    void raiseEvent(Event& args);

#if DEBUG
    void log(const std::string& message);
#endif
};
