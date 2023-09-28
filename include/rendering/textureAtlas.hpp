#pragma once
#include <array>

#include <glm/glm.hpp>

struct TextureAtlas {
  protected:
    float halfTexelSizeU, halfTexelSizeV;
    float cellSizeU, cellSizeV;

    int getMipmapsCount() const;

  public:
    float width, height;
    int rows, cols;

    TextureAtlas(float width, float height, int rows, int cols);

    std::array<glm::vec2, 4> getQuatTextureCoords(int row, int col) const;
};