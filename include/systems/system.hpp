#pragma once

#include "../game.hpp"

#include <entt/entt.hpp>

class System {
  protected:
    entt::registry& registry;
    entt::dispatcher& eventDispatcher;

    Game* game;

    virtual void init();

  public:
    System(Game* game);

    virtual void update(float dt);
};