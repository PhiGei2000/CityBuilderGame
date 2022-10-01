#pragma once
#include "component.hpp"
#include "resources/mesh.hpp"

#include <string>

struct MeshComponent : public Component {
    MeshPtr mesh;

    inline MeshComponent(const MeshPtr& mesh)
        : mesh(mesh) {
    }
};
