#pragma once
#include "system.hpp"

#include <queue>
#include <tuple>

struct BuildEvent;
struct RoadTile;
struct RoadComponent;
struct MeshGeometry;

class RoadSystem : public System {
    private:
        void init();

        std::queue<std::tuple<glm::ivec2, glm::ivec2>> sectionsToBuild;
        std::queue<std::tuple<glm::ivec2, glm::ivec2>> sectionsToPreview;        

        entt::entity roadEntity;              

    public:
        RoadSystem(Game* game);

        void update(float dt);

        std::vector<RoadTile> createTiles(const glm::ivec2& start, const glm::ivec2& end) const;
        void createRoadMesh();
        void createRoadMesh(std::queue<RoadTile>& sections, MeshGeometry* geometry);

        void handleBuildEvent(const BuildEvent& event);
};
