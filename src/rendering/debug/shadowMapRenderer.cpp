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
#include "rendering/debug/shadowMapRenderer.hpp"

#include "rendering/shader.hpp"

#include <GL/glew.h>
#include <math.h>

ShadowMapRenderer::ShadowMapRenderer() {
    shadowDebug = new ShaderProgram("res/shaders/shadowDebug.vert", "res/shaders/shadowDebug.frag");
}

ShadowMapRenderer::~ShadowMapRenderer() {
    delete shadowDebug;
}

void ShadowMapRenderer::render(const ShadowBuffer& buffer) const {
    buffer.bindTextures();

    shadowDebug->use();
    shadowDebug->setInt("shadowMaps", ShadowBuffer::depthMapOffset);

    int columnsCount = std::ceil(std::sqrt(Configuration::SHADOW_CASCADE_COUNT));
    int rowsCount = std::ceil(static_cast<float>(Configuration::SHADOW_CASCADE_COUNT) / columnsCount);

    float strideX = 2.0f / columnsCount;
    float strideY = 2.0f / rowsCount;
    for (int iy = 0; iy < rowsCount; iy++) {
        for (int ix = 0; ix < columnsCount; ix++) {
            int shadowMapID = ix + columnsCount * iy;
            // return if all maps are rendered
            if (shadowMapID >= Configuration::SHADOW_CASCADE_COUNT) {
                return;
            }

            float x = ix * strideX - 1.0f;
            float y = iy * strideY - 1.0f;

            shadowDebug->setInt("layerId", shadowMapID);
            quad.draw(x, y, strideX, strideY);
        }
    }
}
