#include "systems/system.hpp"

void System::init() {
}

void System::destroy() {
}

System::System(Game* game)
    : game(game),
      registry(game->getRegistry()),
      eventDispatcher(game->getEventDispatcher()),
      resourceManager(game->getResourceManager()) {
}

System::~System() {
    destroy();
}

void System::update(float dt) {
}
