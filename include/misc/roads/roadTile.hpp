#pragma once
#include "roadTypes.hpp"

#include <array>
#include <vector>
#include <glm/glm.hpp>

struct RoadTile {
    using Connections = std::array<bool, 4>;

    static constexpr glm::ivec2 undefinedPosition = glm::ivec2(-1);

    glm::ivec2 position;    

    Connections connections;

    RoadTile();

    /// @brief Creates a new road tile at the specified position
    /// @param position The position of the road tile
    /// @param type
    /// @param connections
    RoadTile(const glm::ivec2& position, const Connections& connections = {false, false, false, false});

    /// @brief Determines how often the road tile has to rotated by 90 degrees to fit the connected roads.
    /// @return A value beween 0 and 4 that specifies how often the road tile has to be rotated.
    int getRotation() const;

    /// @brief Determines the road type based on the connections
    /// @return The determined road type
    RoadType getType() const;

    /// @brief Merges the specified connections to the road section.
    /// @param connections 
    void addConnections(const Connections& connections);
};
