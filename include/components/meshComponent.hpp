#pragma once
#include "rendering/geometry.hpp"
#include "rendering/material.hpp"
#include "rendering/shader.hpp"

#include <string>

struct MeshComponent {
    std::shared_ptr<Geometry> geometry;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Material> material;

    inline MeshComponent(std::shared_ptr<Geometry> geometry, std::shared_ptr<Shader> shader, std::shared_ptr<Material> material)
        : geometry(geometry), shader(shader), material(material) {
    }
};

struct MultiMeshComponent {
    std::unordered_map<std::string, MeshComponent> meshes;

    inline MultiMeshComponent(std::initializer_list<std::pair<const std::string, MeshComponent>> meshes)
        : meshes(meshes) {
    }
};
