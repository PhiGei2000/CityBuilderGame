#pragma once
#include <map>
#include <string>

class Configuration {
  public:
    /// @brief The size of one building cell in meters
    static constexpr float cellSize = 5.0f;
    /// @brief The size of one chunk in meters
    static constexpr float chunkSize = 200.0f;    

    static constexpr int maxCars = 1;
    static constexpr float carVelocity = 1.0f;

    static constexpr unsigned int SHADOW_BUFFER_WIDTH = 4096;
    static constexpr unsigned int SHADOW_BUFFER_HEIGHT = 4096;

    static constexpr unsigned int SHADOW_CASCADE_COUNT = 3;
    static constexpr float CASCADE_FAR_PLANE_FACTORS[SHADOW_CASCADE_COUNT] = {0.1f, 0.5f, 1.0f};
};