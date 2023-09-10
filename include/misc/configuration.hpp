#pragma once
#include <map>
#include <string>

class Configuration {
  public:
    static constexpr int gridSize = 5;
    static constexpr int worldSize = 200;

    static constexpr int maxCars = 1;
    static constexpr float carVelocity = 1.0f;

    static constexpr unsigned int SHADOW_BUFFER_WIDTH = 4096;
    static constexpr unsigned int SHADOW_BUFFER_HEIGHT = 4096;

    static constexpr unsigned int SHADOW_CASCADE_COUNT = 4;
    static constexpr float CASCADE_FAR_PLANE_FACTORS[SHADOW_CASCADE_COUNT] = {0.1f,0.25f, 0.5f, 1.0f};
};