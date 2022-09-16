#pragma once
#include "system.hpp"

#include <queue>

struct BuildEvent;
struct RoadSection;

class RoadSystem : public System {
    private:
        void init();

        std::queue<RoadSection> sectionsToBuild;
        std::queue<RoadSection> sectionsToCreateMesh;

        entt::entity roadEntity;

    public:
        RoadSystem(Game* game);

        void update(float dt);

        std::vector<RoadSection> createSection(const glm::ivec2& start, const glm::ivec2& end) const;
        void createRoadMesh();

        void handleBuildEvent(const BuildEvent& event);
};
