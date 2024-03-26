/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "rendering/geometryData.hpp"

GeometryData::GeometryData() {
}

GeometryData::GeometryData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, bool culling)
    : indices(indices), vertices(vertices), culling(culling) {
    calculateTangentSpace();
}

GeometryData GeometryData::merge(const GeometryData& first, const GeometryData& second) {
    assert(first.culling == second.culling);

    GeometryData result = first;

    unsigned int indexOffset = result.vertices.size();
    for (const Vertex& vert : second.vertices) {
        result.vertices.push_back(vert);
    }

    for (const unsigned int index : second.indices) {
        result.indices.push_back(indexOffset + index);
    }

    return result;
}

void GeometryData::addData(const GeometryData& data) {
    int indexOffset = vertices.size();

    for (const Vertex& vertex : data.vertices) {
        this->vertices.push_back(vertex);
    }

    for (const int index : data.indices) {
        this->indices.push_back(index + indexOffset);
    }
}

GeometryData GeometryData::transformVertices(const std::function<Vertex(const Vertex&)>& transform) const {
    GeometryData transformedData;
    transformedData.indices = indices;

    for (const Vertex& vert : vertices) {
        transformedData.vertices.emplace_back(transform(vert));
    }

    return transformedData;
}

GeometryData GeometryData::transformVertices(const glm::mat4& transform) const {
    GeometryData transformedData;
    transformedData.indices = indices;

    const glm::mat3& normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform)));
    for (const Vertex& vert : vertices) {
        const glm::vec3& position = glm::vec3(transform * glm::vec4(vert.position, 1.0f));
        const glm::vec3& normal = normalMatrix * vert.normal;
        const glm::vec3& tangent = normalMatrix * vert.tangent;
        const glm::vec3& bitangent = normalMatrix * vert.bitangent;

        transformedData.vertices.emplace_back(position, vert.texCoord, normal, tangent, bitangent);
    }

    return transformedData;
}

std::pair<glm::vec3, glm::vec3> GeometryData::calculateTangentSpace(const std::array<Vertex, 3>& triangle) {
    const glm::vec3& edge1 = triangle[1].position - triangle[0].position;
    const glm::vec3& edge2 = triangle[2].position - triangle[0].position;

    const glm::vec2& deltaUV1 = triangle[1].texCoord - triangle[0].texCoord;
    const glm::vec2& deltaUV2 = triangle[2].texCoord - triangle[0].texCoord;

    const float f = 1 / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    const glm::vec3& tangent = glm::normalize(f * (deltaUV2.y * edge1 - deltaUV1.y * edge2));
    const glm::vec3& bitangent = glm::normalize(f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2));

    return std::make_pair(tangent, bitangent);
}

void GeometryData::calculateTangentSpace(Vertex& v1, Vertex& v2, Vertex& v3) {
    const auto& [tangent, bitangent] = calculateTangentSpace(std::array<Vertex, 3>{v1, v2, v3});

    v1.tangent = tangent;
    v1.bitangent = bitangent;
    v2.tangent = tangent;
    v2.bitangent = bitangent;
    v3.tangent = tangent;
    v3.bitangent = bitangent;
}

void GeometryData::calculateTangentSpace() {
    for (int i = 0; i < indices.size(); i += 3) {
        // TODO: Check if two tangent spaces are different if and only if the normal vectors are different
        calculateTangentSpace(this->vertices[indices[i]], this->vertices[indices[i + 1]], this->vertices[indices[i + 2]]);
    }
}

#if DEBUG
#include <fstream>
#include <set>
#include <sstream>
#include <iostream>

void GeometryData::save(const std::string& filename) const {
    std::cout << "save mesh to: " << filename << std::endl;

    std::stringstream outString;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    std::vector<std::array<std::tuple<unsigned int, unsigned int, unsigned int>, 3>> faces;

    for (const auto& vertex : vertices) {
        positions.push_back(vertex.position);
        texCoords.push_back(vertex.texCoord);
        normals.push_back(vertex.normal);
    }

    for (const auto& position : positions) {
        outString << "v " << position.x << " " << position.y << " " << position.z << std::endl;
    }

    for (const auto& texCoord : texCoords) {
        outString << "vt " << texCoord.x << " " << texCoord.y << std::endl;
    }

    for (const auto& normal : normals) {
        outString << "vn " << normal.x << " " << normal.y << " " << normal.z << std::endl;
    }

    for (int i = 0; i < indices.size(); i += 3) {
        // faces
        outString << "f ";
        for (int j = 0; j < 3; j++) {
            outString << indices[i + j] + 1 << "/"
                      << indices[i + j] + 1 << "/"
                      << indices[i + j] + 1 << " ";
        }
        outString << std::endl;
    }

    std::ofstream file;
    file.open(filename);

    file << outString.str();
    file.close();
}
#endif
