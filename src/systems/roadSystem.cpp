#include "systems/roadSystem.hpp"

#include "events/buildEvent.hpp"
#include "misc/roads/roadSection.hpp"

RoadSystem::RoadSystem(Game* game)
    : System(game) {

    eventDispatcher.sink<BuildEvent>()
        .connect<&RoadSystem::handleBuildEvent>(*this);

    init();
}

void RoadSystem::init() {
}

void RoadSystem::update(float dt) {
    while (sectionsToBuild.size() > 0) {
        // create section mesh
    }
}

void RoadSystem::handleBuildEvent(const BuildEvent& event) {
    if (event.type != BuildingType::ROAD)
        return;

    if (event.action == BuildAction::END) {
        // end of the road section
        sectionsToBuild.emplace(RoadSection{
            event.buildingStartPosition,
            event.gridPosition,
            RoadType::UNDEFINED});
    }
}
