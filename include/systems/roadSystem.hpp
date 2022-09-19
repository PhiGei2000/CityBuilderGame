#pragma once
#include "system.hpp"

#include <queue>
#include <tuple>

struct BuildEvent;
struct RoadTile;
struct RoadComponent;

class RoadSystem : public System {
    private:
        void init();

        std::queue<std::tuple<glm::ivec2, glm::ivec2>> sectionsToBuild;
        std::queue<RoadTile> sectionsToCreateMesh;

        entt::entity roadEntity;        

    public:
        RoadSystem(Game* game);

        void update(float dt);

        std::vector<RoadTile> createTiles(const glm::ivec2& start, const glm::ivec2& end) const;
        void createRoadMesh();

        void handleBuildEvent(const BuildEvent& event);
};
