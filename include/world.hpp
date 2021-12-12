#pragma once
#include "rendering/geometryData.hpp"
#include "rendering/camera.hpp"
#include "resourceManager.hpp"
#include "streets.hpp"

#include <vector>

namespace trafficSimulation {
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