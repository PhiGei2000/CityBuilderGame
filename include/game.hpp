#pragma once
#include "resources/resourceManager.hpp"

#include "misc/typedefs.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

class System;
class Application;

enum class GameState
{
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

    void loadResources();

    void init();

    GameState state = GameState::RUNNING;

  public:
    entt::entity sun;

    Game(Application* app);

    bool debugMode = false;

    entt::registry& getRegistry();
    entt::dispatcher& getEventDispatcher();
    ResourceManager& getResourceManager();

    void update(float dt);

    int getKey(int key) const;

    glm::vec2 getMousePos() const;

    void setState(GameState state);
    GameState getState() const;

    template<typename Event>
    void raiseEvent(Event& args);
};
