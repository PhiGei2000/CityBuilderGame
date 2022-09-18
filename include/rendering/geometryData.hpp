#pragma once
#include "rendering/vertex.hpp"

#include <functional>
#include <unordered_set>
#include <vector>

struct GeometryData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    static GeometryData merge(const GeometryData& first, const GeometryData& second);

    void addData(const GeometryData& data);

    GeometryData transformVertices(const std::function<Vertex(const Vertex&)>& transformation) const;
    GeometryData transformVertices(const glm::mat4& transform) const;
};