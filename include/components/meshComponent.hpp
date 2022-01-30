#pragma once
#include "rendering/mesh.hpp"

#include <string>

typedef Mesh MeshComponent;

struct MultiMeshComponent {
    std::unordered_map<std::string, Mesh> meshes;
};
