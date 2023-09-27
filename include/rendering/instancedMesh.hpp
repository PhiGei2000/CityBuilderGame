#pragma once

#include "components/transformationComponent.hpp"
#include "rendering/instanceBuffer.hpp"

#include <vector>

struct InstancedMesh {
    std::vector<TransformationComponent> transformations;
    InstanceBuffer instanceBuffer;

    inline InstancedMesh() {
    }

    inline InstancedMesh(const std::vector<TransformationComponent>& transformations)
        : transformations(transformations) {
        instanceBuffer.fillBuffer(transformations);
    }
};