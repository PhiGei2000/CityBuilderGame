#include "streetBuilder.hpp"
#include "world.hpp"

namespace trafficSimulation {

    StreetBuilder::StreetBuilder(World* world)
        : world(world) {
    }

    void StreetBuilder::buildSegment(glm::ivec2 pos, Direction direction, int length) {
        if (length == 0) {
            world->createStreet(pos.x, pos.y, StreetID::SINGLE_STREET);
            return;
        }

        // set street begin and loop paramters
        int currX = pos.x, currY = pos.y;
        int xStep = 0, yStep = 0;
        StreetID type = StreetID::NO_STREET;
        switch (direction) {
        case Direction::UP:
            world->createStreet(currX, currY, StreetID::STREET_END_BOTTOM);
            type = StreetID::VERTICAL_STREET;
            yStep = 1;
            break;
        case Direction::LEFT:
            world->createStreet(currX, currY, StreetID::STREET_END_RIGHT);
            type = StreetID::HORIZONTAL_STREET;
            xStep = -1;
            break;
        case Direction::DOWN:
            world->createStreet(currX, currY, StreetID::STREET_END_TOP);
            type = StreetID::VERTICAL_STREET;
            yStep = -1;
            break;
        case Direction::RIGHT:
            world->createStreet(currX, currY, StreetID::STREET_END_LEFT);
            type = StreetID::HORIZONTAL_STREET;
            xStep = 1;
            break;
        }

        currX += xStep;
        currY += yStep;

        for (int i = 1; i < length - 1; i++) {
            world->createStreet(currX, currY, type);

            currX += xStep;
            currY += yStep;
        }

        switch (direction) {
        case Direction::UP:
            world->createStreet(currX, currY, StreetID::STREET_END_TOP);
            break;
        case Direction::LEFT:
            world->createStreet(currX, currY, StreetID::STREET_END_LEFT);
            break;
        case Direction::DOWN:
            world->createStreet(currX, currY, StreetID::STREET_END_BOTTOM);
            break;
        case Direction::RIGHT:
            world->createStreet(currX, currY, StreetID::STREET_END_RIGHT);
            break;
        }
    }

    void StreetBuilder::build(const Path& p) {
        int segmentsCount = p.size() - 1;

        // begin iteration and get first point
        auto it = p.begin();
        glm::ivec2 curr = *it;
        it++;

        // loop through all points
        while (it != p.end()) {
            glm::ivec2 next = *it;

            int dx = next.x - curr.x;
            int dy = next.y - curr.y;
            
            if (dx != 0) {
                if (dx > 0) {
                    buildSegment(curr, Direction::RIGHT, dx + 1);
                }
                else {
                    buildSegment(curr, Direction::LEFT, -dx + 1);
                }
            }

            if (dy != 0) {
                if (dy > 0) {
                    buildSegment(curr, Direction::UP, dy + 1);
                }
                else {
                    buildSegment(curr, Direction::DOWN, -dy + 1);
                }
            }

            curr = next;
            it++;
        }
    }

} // namespace trafficSimulation