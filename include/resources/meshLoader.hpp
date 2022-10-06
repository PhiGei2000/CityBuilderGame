#pragma once

#include "rendering/geometryData.hpp"
#include "resources/mesh.hpp"

#include <array>
#include <sstream>
#include <string>

class MeshLoader {
  private:
    struct VertexData {
      std::vector<glm::vec3> positions;
      std::vector<glm::vec2> texCoords;
      std::vector<glm::vec3> normals;
    };

    using FaceIndices = std::array<glm::uvec3, 3>;
    
    /// @brief A collection of the faces grouped by the used material
    using FaceData = std::unordered_map<std::string, std::vector<FaceIndices>>;

    static std::stringstream readLine(std::stringstream& s);    

    static VertexData parseVertexData(std::stringstream& s);
    static FaceData parseFaceData(std::stringstream& s);

    static glm::vec2 parseVec2(std::stringstream& s);
    static glm::vec3 parseVec3(std::stringstream& s);    

  public:
    static TexturePtr loadTexture(const std::string& filename, int format = GL_RGBA);

    static std::unordered_map<std::string, MaterialPtr> loadMaterials(const std::string& filename);

    static MeshPtr loadMesh(const std::string& filename, ShaderPtr shader);
};