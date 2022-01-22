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
    int indexOffset = indices.size();

    for (const Vertex& vertex : data.vertices) {
        this->vertices.push_back(vertex);
    }

    for (const int index : data.indices) {
        this->indices.push_back(index + indexOffset);
    }
}

GeometryData GeometryData::transformVertices(const GeometryData& data, const std::function<Vertex(const Vertex&)>& transform) {
    GeometryData transformedData;
    transformedData.indices = data.indices;

    for (const Vertex& vert : data.vertices) {
        transformedData.vertices.emplace_back(transform(vert));
    }

    return transformedData;
}
