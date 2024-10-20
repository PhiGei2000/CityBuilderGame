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
#include <map>

#include "misc/roads/roadTile.hpp"

struct RoadMeshComponent {
    std::map<RoadTypes, std::map<RoadTileTypes, InstancedMesh<glm::mat4>>> roadMeshes;

#if DEBUG
    Geometry* graphDebugMesh = new Geometry(VertexAttributes{VertexAttribute{3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0u}}, GL_LINES);
#endif
};