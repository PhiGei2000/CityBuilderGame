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
#include "systems/roadSystem.hpp"

#include "components/components.hpp"
#include "events/buildEvent.hpp"
#include "misc/configuration.hpp"
#include "misc/coordinateTransform.hpp"
#include "misc/direction.hpp"
#include "misc/utility.hpp"
#include "resources/roadPack.hpp"

RoadSystem::RoadSystem(Game* game)
    : System(game) {

    eventDispatcher.sink<BuildEvent>()
        .connect<&RoadSystem::handleBuildEvent>(*this);

    init();
}



void RoadSystem::update(float dt) {
    RoadPackPtr roadPack = resourceManager.getResource<RoadPack>("BASIC_STREETS");

    registry.view<RoadComponent, BuildingComponent, RoadMeshComponent>().each([&](entt::entity entity, RoadComponent& road, BuildingComponent& building, RoadMeshComponent& mesh) {
        // if (entity != roadEntity && !building.preview) {
        //     // merge builded road into "world" road
        //     RoadComponent& worldRoad = registry.get<RoadComponent>(roadEntity);


        // }
        // else {
            if (road.meshOutdated) {
                createRoadMesh(road, mesh);

                road.meshOutdated = false;
            }
        // }
    });
}

void RoadSystem::createRoadMesh(const RoadComponent& road, RoadMeshComponent& geometry) const {

}

void RoadSystem::handleBuildEvent(const BuildEvent& event) {
    // if (event.type != BuildingType::ROAD)
    //     return;

    // int segmentsCount = event.positions.size() - 1;

    // previewGraph.clear();
    // if (event.action == BuildAction::END) {
    //     for (int i = 0; i < segmentsCount; i++) {
    //         sectionsToBuild.emplace(event.positions[i], event.positions[i + 1]);
    //     }
    // }
    // else if (event.action == BuildAction::PREVIEW) {
    //     for (int i = 0; i < segmentsCount; i++) {
    //         sectionsToPreview.emplace(event.positions[i], event.positions[i + 1]);
    //     }
    // }
}
