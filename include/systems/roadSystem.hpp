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

#include "misc/roads/roadGraph.hpp"
#include "resources/roadPack.hpp"

#include <queue>
#include <tuple>

#include <glm/gtx/hash.hpp>

struct BuildEvent;
struct RoadComponent;
struct MeshGeometry;

class RoadSystem : public System {
  private:
    void init();

    std::queue<std::pair<glm::ivec2, glm::ivec2>> sectionsToBuild;
    std::queue<std::pair<glm::ivec2, glm::ivec2>> sectionsToPreview;

    RoadGraph previewGraph;

    entt::entity roadEntity;

  public:
    RoadSystem(Game* game);

    void update(float dt);

    void createRoadMesh(const RoadGraph& graph, MeshGeometry* geometry, ResourcePtr<RoadPack> roadPack) const;

    void clearRoadGraph(RoadGraph& graph) const;

    void handleBuildEvent(const BuildEvent& event);
};
