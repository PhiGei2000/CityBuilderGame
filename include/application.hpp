#pragma once
#include "rendering/window.hpp"
#include "world.hpp"

#include <entt/entt.hpp>

namespace trafficSimulation {
    class Application {
      private:
        Window window;
        World world;

        entt::registry registry;
        entt::dispatcher eventDispatcher;

        void init();

      public:
        Application();

        void run();
    };
} // namespace trafficSimulation