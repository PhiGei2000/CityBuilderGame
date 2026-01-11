#pragma once
#include "misc/roads/roadSpecs.hpp"
#include "misc/roads/roadTile.hpp"
#include "resources/mesh.hpp"

#include "rendering/material.hpp"
#include "rendering/shader.hpp"

#include <map>

struct InstanceBuffer;

struct RoadPack {
    std::string name;
    std::string icon;
    RoadSpecs specs;

    MeshPtr roadGeometries;

    ResourcePtr<Material> material;

    RoadPack(const std::string& name, const RoadSpecs& specs, ResourcePtr<Material> material, ResourcePtr<Shader> shader, const std::string& icon = "");
};

using RoadPackPtr = ResourcePtr<RoadPack>;
