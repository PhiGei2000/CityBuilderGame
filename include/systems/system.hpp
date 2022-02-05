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

  public:
    System(Game* game);

    virtual void update(float dt);
};