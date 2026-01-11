#pragma once
#include "rendering/instanceBuffer.hpp"
#include "resources/mesh.hpp"

#include <vector>

template<typename TData>
struct InstancedData {
    std::vector<TData> transformations;
    InstanceBuffer instanceBuffer;

    inline InstancedData() {
    }

    inline ~InstancedData() {
    }

    inline InstancedData(const std::vector<TData>& transformations)
        : transformations(transformations) {
        instanceBuffer.fillBuffer(transformations);
    }
};

template<typename TData>
struct InstancedMesh {
    MeshPtr mesh;
    std::unordered_map<std::string, InstancedData<TData>> instances;
};
