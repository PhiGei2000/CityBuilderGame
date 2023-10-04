#pragma once
#include <map>
#include <string>

class Configuration {
  public:
    /// @brief The size of one building cell in meters
    static constexpr int cellSize = 5;
    /// @brief The size of one chunk in meters
    static constexpr int chunkSize = 200;
    /// @brief The number of cells in a chunk in one direction
    static constexpr int cellsPerChunk = chunkSize / cellSize;

    /// @brief The distance of the camera above the terrain
    static constexpr float cameraHeight = 15.0f;

    /// @brief Maximum number of cars
    static constexpr int maxCars = 1;
    /// @brief Velocity of one car
    static constexpr float carVelocity = 1.0f;

    static constexpr unsigned int SHADOW_BUFFER_WIDTH = 4096;
    static constexpr unsigned int SHADOW_BUFFER_HEIGHT = 4096;

    static constexpr unsigned int SHADOW_CASCADE_COUNT = 4;
    static constexpr float CASCADE_FAR_PLANE_FACTORS[SHADOW_CASCADE_COUNT] = {0.05f, 0.125f, 0.25f, 0.5f};
};