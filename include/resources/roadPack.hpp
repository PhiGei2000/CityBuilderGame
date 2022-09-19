#pragma once
#include "misc/roads/roadTypes.hpp"
#include "misc/typedefs.hpp"

#include "rendering/geometryData.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "rendering/material.hpp"

#include <map>

struct RoadPack {
    std::map<RoadType, GeometryData> roadGeometries;    

    ResourcePtr<Shader> shader;
    ResourcePtr<Material> material;
};
