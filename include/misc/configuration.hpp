#pragma once

class Configuration {
  public:
    static constexpr int gridSize = 5;
    static constexpr int worldSize = 200;

    static constexpr int maxCars = 1;
    static constexpr float carVelocity = 1.0f;

    static constexpr unsigned int SHADOW_BUFFER_WIDTH = 1024;
    static constexpr unsigned int SHADOW_BUFFER_HEIGHT = 1024;

    static constexpr unsigned int SHADOW_BUFFER_SPLIT_COUNT = 2;
    static constexpr float CASCADE_FAR_PLANES_FACTORS[SHADOW_BUFFER_SPLIT_COUNT] = {0.75f, 1.0f};
};