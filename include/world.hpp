#pragma once
#include "rendering/camera.hpp"
#include "rendering/geometryData.hpp"
#include "resourceManager.hpp"
#include "streets.hpp"

#include <vector>

namespace trafficSimulation {
    // TODO: Remove this and switch to entitiy component system
    class World {
      private:
        bool changed = false;
        ResourceManager resourceManager;
        Camera camera;

      public:
        static constexpr int size = 64;
        StreetID** data;

        void init();
        void destroy();

        StreetID createStreet(int x, int y, StreetID type);

        GeometryData getVertices();
        bool geometryChanged() const;

        const ResourceManager& getResourceManager() const;

        Camera& getCamera();
        const Camera& getCamera() const;

        void handleKey();
    };
} // namespace trafficSimulation