#include "rendering/debug/shadowMapRenderer.hpp"

#include "rendering/shader.hpp"

#include <GL/glew.h>
#include <math.h>

ShadowMapRenderer::ShadowMapRenderer() {
    shadowDebug = new Shader("res/shaders/shadowDebug.vert", "res/shaders/shadowDebug.frag");
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
