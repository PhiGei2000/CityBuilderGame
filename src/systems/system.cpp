#include "systems/system.hpp"

void System::init() {
}

System::System(Game* game)
    : game(game),
      registry(game->getRegistry()),
      eventDispatcher(game->getEventDispatcher()) {
}

void System::update(int dt) {
}