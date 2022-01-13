#pragma once
#include "rendering/vertex.hpp"

#include <unordered_set>
#include <vector>

struct GeometryData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    static GeometryData merge(const GeometryData& first, const GeometryData& second);
};