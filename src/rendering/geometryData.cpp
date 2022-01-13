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