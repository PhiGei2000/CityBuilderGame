#pragma once

#include "rendering/geometry.hpp"

#include <string>

namespace trafficSimulation {
    class ModelLoader {
      private:
        static glm::vec3 parseVec3(const std::string& str, int startIndex = 0);
        static glm::vec2 parseVec2(const std::string& str, int startIndex = 0);

      public:
        static Geometry load(const std::string& filename);
    };
} // namespace trafficSimulation