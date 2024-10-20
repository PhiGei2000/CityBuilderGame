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
#include "misc/roads/graph.hpp"
#include "misc/roads/roadSpecs.hpp"
#include "misc/roads/roadTile.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <array>

class RoadPathGenerator {
  public:
    static RoadPath generateEdgePath(const RoadGraph::EdgeType& edge, const RoadSpecs& specs);

    static std::array<std::array<RoadPath, 4>, 4> generateNodePaths(const RoadGraph::NodeType& node, const RoadSpecs& specs, const RoadTile& tile);
};
