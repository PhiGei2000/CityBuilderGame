#pragma once
#include "misc/streetTypes.hpp"
#include "misc/typedefs.hpp"

#include "rendering/geometryData.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"

#include "unordered_map"

struct StreetPack {
    std::unordered_map<StreetType, GeometryData> streetGeometries;
    // float height;
    // float pathwayHeight;
    // float pathwayWidth;

    ResourcePtr<Shader> shader;
    ResourcePtr<Texture> texture;
};
