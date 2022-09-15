#pragma once
#include "system.hpp"

#include <queue>

struct BuildEvent;
struct RoadSection;

class RoadSystem : public System {
    private:
        void init();

        std::queue<RoadSection> sectionsToBuild;

    public:
        RoadSystem(Game* game);

        void update(float dt);

        void handleBuildEvent(const BuildEvent& event);
};
