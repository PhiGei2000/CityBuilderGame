#include "systems/system.hpp"

void System::init() {
}

System::System(Game* game)
    : game(game),
      registry(game->getRegistry()),
      eventDispatcher(game->getEventDispatcher()),
      resourceManager(game->getResourceManager()) {
}

void System::update(float dt) {
}