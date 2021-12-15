#include "systems/system.hpp"

namespace trafficSimulation::systems {
    void System::init() {
    }

    System::System(Application* app)
        : registry(app->getRegistry()),
          eventDispatcher(app->getEventDispatcher()) {
    }

    void System::update(int dt) {
    }
} // namespace trafficSimulation::systems