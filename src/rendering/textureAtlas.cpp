#include "rendering/textureAtlas.hpp"

TextureAtlas::TextureAtlas(float width, float height, int rows, int cols)
    : width(width), height(height), rows(rows), cols(cols) {
    halfTexelSizeU = 8.0f / width;
    halfTexelSizeV = 8.0f / height;

    cellSizeU = 1.0f / cols;
    cellSizeV = 1.0f / rows;
}

int TextureAtlas::getMipmapsCount() const {
    return 1 + floor(log2(std::max(width, height)));
}

std::array<glm::vec2, 4> TextureAtlas::getQuadTextureCoords(int index) const {
    return getQuadTextureCoords(index / rows, index % rows);
}

std::array<glm::vec2, 4> TextureAtlas::getQuadTextureCoords(int row, int col) const {
    return {
        glm::vec2(col * cellSizeU + halfTexelSizeU, (rows - row - 1) * cellSizeV + halfTexelSizeV),
        glm::vec2((col + 1) * cellSizeU - halfTexelSizeU, (rows - row - 1) * cellSizeV + halfTexelSizeV),
        glm::vec2(col * cellSizeU + halfTexelSizeU, (rows - row) * cellSizeV - halfTexelSizeV),
        glm::vec2((col + 1) * cellSizeU - halfTexelSizeU, (rows - row) * cellSizeV - halfTexelSizeV),
    };
}
