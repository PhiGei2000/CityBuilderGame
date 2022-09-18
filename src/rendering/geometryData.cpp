#include "rendering/geometryData.hpp"

GeometryData GeometryData::merge(const GeometryData& first, const GeometryData& second) {
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

        transformedData.vertices.emplace_back(position, vert.texCoord, normal);
    }

    return transformedData;
}
