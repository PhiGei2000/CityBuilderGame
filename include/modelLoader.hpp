#pragma once

#include "rendering/geometry.hpp"

#include <string>
#include <sstream>

namespace trafficSimulation {
    class ModelLoader {
      private:
        using FaceIndices = std::array<std::tuple<int unsigned, unsigned int, unsigned int>, 3>;

        static void processFaces(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texCoords, const std::vector<glm::vec3>& normals, const std::vector<FaceIndices>& faces, GeometryData& data);

        static FaceIndices parseVertexIndices(const std::string& str);        

      public:
        static Geometry* load(const std::string& filename);
    };
} // namespace trafficSimulation