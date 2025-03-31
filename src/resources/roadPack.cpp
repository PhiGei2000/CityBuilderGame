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

#include "resources/roadPack.hpp"

#include "resources/roadGeometryGenerator.hpp"

RoadPack::RoadPack(const std::string& name, const RoadSpecs& specs, MaterialPtr material, ShaderPtr shader, const std::string& icon)
    : specs(specs), roadGeometries(new Mesh(shader)), name(name), icon(icon) {
    const auto& geometries = RoadGeometryGenerator::generateRoadPackGeometries(specs);

    for (const auto& [type, geometry] : geometries) {
        roadGeometries->geometries[getRoadTileTypeName(type)] = std::vector{std::make_pair(material, geometry)};
    }
}