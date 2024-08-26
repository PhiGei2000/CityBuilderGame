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
#include "component.hpp"
#include "misc/configuration.hpp"
#include "misc/direction.hpp"
#include "misc/roads/roadGraph.hpp"
#include "misc/roads/roadSpecs.hpp"
#include "misc/roads/roadTile.hpp"

#include <glm/gtx/hash.hpp>
#include <map>
#include <string>
#include <unordered_map>

/// @brief A component that represents roads in one chunk
struct RoadComponent : public AssignableComponent {
    /// @brief This array holds
    RoadTile roadTiles[Configuration::cellsPerChunk][Configuration::cellsPerChunk];

    /// @brief Borders in north, east, south and west direction
    bool borders[4][Configuration::cellsPerChunk];

    /// @brief `true` if the road was updated and the mesh must be recreated
    bool meshOutdated = false;

    /// @brief A graph data structure that represents the roads and crossings
    RoadGraph graph;

    RoadComponent();

    RoadComponent(const RoadTile (&tiles)[Configuration::cellsPerChunk][Configuration::cellsPerChunk]);

    RoadComponent(const RoadTile (&tiles)[Configuration::cellsPerChunk][Configuration::cellsPerChunk], const bool (&borders)[4][Configuration::cellsPerChunk]);

    /// @brief Deletes all road information
    void clear();

    /// @brief Fills the area between the given positions with roads
    /// @param position1 The first position
    /// @param position2 The second position
    void setRoad(const glm::uvec2& position1, const glm::uvec2& position2);

    bool isConnected(const glm::ivec2& pos, Direction dir = Direction::UNDEFINED) const;

    void updateRoadTypes(const std::map<std::string, RoadSpecs>& specs);

    void updateRoad(const glm::ivec2& pos, const std::map<std::string, RoadSpecs>& specs);

    static constexpr RoadTile getTileType(const bool (&connections)[4]);

    /// @brief Determines which positions are road nodes. A road node is every type of road except of straight road pieces
    /// @return A collection of the positions
    std::unordered_set<glm::ivec2> getNodes() const;

    void updateRoadGraph(const std::map<std::string, RoadSpecs>& specs);

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        RoadComponent& road = registry.emplace<RoadComponent>(entity, roadTiles);
        road.meshOutdated = true;
    }
};
