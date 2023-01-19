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

    /// @brief Holds index information about a vertex.
    struct VertexIndices {
        unsigned int positionIndex;
        unsigned int texCoordIndex;
        unsigned int normalIndex;

        inline VertexIndices()
            : positionIndex(INT_MAX), texCoordIndex(INT_MAX), normalIndex(INT_MAX) {
        }

        inline VertexIndices(unsigned int positionIndex, unsigned int texCoordIndex, unsigned int normalIndex)
            : positionIndex(positionIndex), texCoordIndex(texCoordIndex), normalIndex(normalIndex) {
        }

        inline bool operator==(const VertexIndices& other) const {
            return positionIndex == other.positionIndex && texCoordIndex == other.texCoordIndex && normalIndex == other.normalIndex;
        }

        inline bool operator!=(const VertexIndices& other) const {
            return !operator==(other);
        }

        inline VertexIndices operator+(const VertexIndices& rhs) const {
            return VertexIndices(positionIndex + rhs.positionIndex, texCoordIndex + rhs.texCoordIndex, normalIndex + rhs.normalIndex);
        }

        inline VertexIndices operator-(const VertexIndices& rhs) const {
            return VertexIndices(positionIndex - rhs.positionIndex, texCoordIndex - rhs.texCoordIndex, normalIndex - rhs.normalIndex);
        }

        inline VertexIndices& operator+=(const VertexIndices& rhs) {
            positionIndex += rhs.positionIndex;
            texCoordIndex += rhs.texCoordIndex;
            normalIndex += rhs.normalIndex;

            return *this;
        }

        inline VertexIndices& operator-=(const VertexIndices& rhs) {
            positionIndex -= rhs.positionIndex;
            texCoordIndex -= rhs.texCoordIndex;
            normalIndex -= rhs.normalIndex;

            return *this;
        }
    };

    using FaceIndices = std::array<VertexIndices, 3>;
    struct FaceDataCollection {
        std::vector<FaceIndices> indicesCulling;
        std::vector<FaceIndices> indicesNonCulling;
    };

    /// @brief A collection of the faces grouped by the used material
    using FaceData = std::unordered_map<std::string, FaceDataCollection>;    

    static std::stringstream readLine(std::stringstream& s);

    static VertexData parseVertexData(std::stringstream& s);
    static FaceData parseFaceData(std::stringstream& s, const VertexIndices& indexOffsets);

    static glm::vec2 parseVec2(std::stringstream& s);
    static glm::vec3 parseVec3(std::stringstream& s);

    static void correctWindingOrder(const VertexData& data, FaceData& faceData);

    static GeometryData processFaces(const std::vector<FaceIndices>& indices, const VertexData& vertData);

  public:
    static TexturePtr loadTexture(const std::string& filename, int format = GL_RGBA);

    static std::unordered_map<std::string, MaterialPtr> loadMaterials(const std::string& filename);

    static MeshPtr loadMesh(const std::string& filename, ShaderPtr shader);
};