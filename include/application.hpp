#pragma once
#include <vector>

#include "resourceManager.hpp"

#include <entt/entt.hpp>

namespace trafficSimulation {
    namespace systems {
        class System;
    }    

    class Application {
      private:
        std::vector<systems::System*> systems;        

        ResourceManager resourceManager;

        entt::registry registry;
        entt::dispatcher eventDispatcher;

        bool stopRequested = false;

        void init();

        void loadResources();

      public:
        Application();

        void run();

        entt::registry& getRegistry();
        entt::dispatcher& getEventDispatcher();
        ResourceManager& getResourceManager();

        template<typename Event>
        void raiseEvent(const Event& args);
    };
} // namespace trafficSimulation