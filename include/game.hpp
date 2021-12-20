#pragma once
#include "resourceManager.hpp"

#include <entt/entt.hpp>

class System;
class Application;

class Game {
    std::vector<System*> systems;

    ResourceManager resourceManager;

    entt::registry registry;
    entt::dispatcher eventDispatcher;

    Application* app;

    void loadResources();

    void init();
  public:
    bool paused;

    Game(Application* app);

    entt::registry& getRegistry();
    entt::dispatcher& getEventDispatcher();
    ResourceManager& getResourceManager();

    void update(float dt);

    int getKey(int key) const;

    template<typename Event>
    void raiseEvent(const Event& args);
};
