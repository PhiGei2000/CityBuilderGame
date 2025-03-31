/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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
