#pragma once

class Configuration {
  public:
    static constexpr int gridSize = 5;
    static constexpr int worldSize = 200;

    static constexpr int maxCars = 1;
    static constexpr float carVelocity = 1.0f;

    static constexpr unsigned int SHADOW_BUFFER_WIDTH = 1024;
    static constexpr unsigned int SHADOW_BUFFER_HEIGHT = 1024;
};