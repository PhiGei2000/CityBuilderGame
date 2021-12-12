#pragma once
#include "world.hpp"

#include <vector>
#include <glm/glm.hpp>

namespace trafficSimulation {
    class World;    

    using Path = std::vector<glm::ivec2>;

    enum class Direction
    {
        UP,
        LEFT,
        DOWN,
        RIGHT
    };

    class StreetBuilder {
      private:
        World* world;

        void buildSegment(glm::ivec2 pos, Direction direction, int lenght);

      public:
        StreetBuilder(World* world);

        void build(const Path& p);
    };
} // namespace trafficSimulation
