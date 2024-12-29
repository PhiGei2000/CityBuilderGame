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
