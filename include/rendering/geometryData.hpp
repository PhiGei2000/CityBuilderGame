#pragma once
#include "rendering/vertex.hpp"

#include <unordered_set>
#include <vector>

struct GeometryData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};