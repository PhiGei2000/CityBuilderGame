#pragma once
#include "resources/mesh.hpp"

#include <string>

struct MeshComponent {
    MeshPtr mesh;

    inline MeshComponent(const MeshPtr& mesh)
        : mesh(mesh) {
    }
};
