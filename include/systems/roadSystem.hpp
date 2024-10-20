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
#include "system.hpp"

#include "misc/roads/graph.hpp"
#include "resources/roadPack.hpp"

#include <queue>
#include <tuple>

#include <glm/gtx/hash.hpp>

struct BuildEvent;
struct RoadComponent;
struct MeshGeometry;
struct RoadMeshComponent;

class RoadSystem : public System {
  protected:
    std::queue<std::pair<glm::ivec2, std::string>> roadsToBuild;
    std::queue<glm::ivec2> chunksToUpdateMesh;

    std::map<std::string, RoadSpecs> roadSpecs;

    void createRoadMesh(const RoadComponent& roadComponent, RoadMeshComponent& geometry) const;

    void init();

  public:
    RoadSystem(Game* game);

    void update(float dt);

    void handleBuildEvent(const BuildEvent& event);
};
