#pragma once

#include "rendering/geometryData.hpp"
#include "resources/mesh.hpp"

#include <sstream>
#include <string>

#if WIN32
#include <array>
#endif

class MeshLoader {
  private:
    using FaceIndices = std::array<std::tuple<int unsigned, unsigned int, unsigned int>, 3>;
    
    static std::stringstream readLine(std::stringstream& ss);

    static void processFaces(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texCoords, const std::vector<glm::vec3>& normals, const std::vector<FaceIndices>& faces, GeometryData& data);

    static FaceIndices parseVertexIndices(const std::string& str);


  public:
    static TexturePtr loadTexture(const std::string& filename);

    static std::unordered_map<std::string, MaterialPtr> loadMaterials(const std::string& filename);

    static MeshPtr loadMesh(const std::string& filename, ShaderPtr shader);
};