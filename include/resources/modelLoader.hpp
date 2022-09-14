#pragma once

#include "misc/typedefs.hpp"

#include "rendering/geometry.hpp"
#include "rendering/geometryData.hpp"
#include "rendering/material.hpp"

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

#if WIN32
#include <array>
#endif

class ModelLoader {
  private:
    using FaceIndices = std::array<std::tuple<int unsigned, unsigned int, unsigned int>, 3>;

    static void processFaces(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texCoords, const std::vector<glm::vec3>& normals, const std::vector<FaceIndices>& faces, GeometryData& data);

    static FaceIndices parseVertexIndices(const std::string& str);

  public:    
    static GeometryData load(const std::string& filename);

    static std::unordered_map<std::string, Material*> loadMaterials(const std::string& filename, const ResourceManager* resourceManager);
};