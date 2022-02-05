#pragma once
#include "misc/streetTypes.hpp"
#include "misc/typedefs.hpp"

#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"

#include "unordered_map"

struct StreetPack {
    std::unordered_map<StreetType, ResourcePtr<Geometry>> streetGeometries;

    ResourcePtr<Shader> shader;
    ResourcePtr<Texture> texture;
};
