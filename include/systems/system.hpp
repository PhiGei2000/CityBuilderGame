#pragma once

#include "../game.hpp"

#include <entt/entt.hpp>

class System {
  protected:
    entt::registry& registry;
    entt::dispatcher& eventDispatcher;

    ResourceManager& resourceManager;

    Game* game;

    virtual void init();

    virtual void destroy();

  public:
    System(Game* game);

    ~System();

    virtual void update(float dt);
};
