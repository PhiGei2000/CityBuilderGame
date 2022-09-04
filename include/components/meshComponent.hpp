#pragma once
#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/material.hpp"

#include <string>

struct MeshComponent {
    std::shared_ptr<Geometry> geometry;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Material> material;
};

struct MultiMeshComponent {
    std::unordered_map<std::string, MeshComponent> meshes;    
};
