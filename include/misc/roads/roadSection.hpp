#pragma once
#include "roadTypes.hpp"

#include <array>
#include <glm/glm.hpp>

struct RoadSection {    
    glm::ivec2 position;
    glm::ivec2 sectionVector = glm::ivec2(0.0f);
    
    std::array<bool, 4> connections;

    /// @brief Creates a new road tile at the specified position
    /// @param position The position of the road tile
    /// @param type 
    /// @param connections 
    RoadSection(const glm::ivec2& position, const std::array<bool, 4>& connections = {false, false, false, false});
    
    /// @brief Creates a new road section from the start to the end position.
    /// @param start Start position
    /// @param end End position
    RoadSection(const glm::ivec2& start, const glm::ivec2& end);

    /// @brief Determines how often the road tile has to rotated by 90 degrees to fit the connected roads.
    /// @return A value beween 0 and 4 that specifies how often the road tile has to be rotated.
    int getRotation() const;

    /// @brief Calculates the rectlinear length of the road section.
    /// @return The rectlinear length of the section vector.
    int getLength() const;

    /// @brief Determines the road type based on the connections
    /// @return The determined road type
    RoadType getType() const;

    /// @brief Determines if the section contains a specific position.
    /// @param pos The position to check
    /// @return true, if the section contains the position, false otherwise.
    bool containsPosition(const glm::ivec2& pos) const;
};
