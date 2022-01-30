#pragma once
#include "rendering/mesh.hpp"

#include <glm/glm.hpp>
#include <string>

typedef Mesh MeshComponent;

struct MultiMeshComponent {
    std::vector<Mesh> meshes;
};
