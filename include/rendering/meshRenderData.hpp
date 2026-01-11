#pragma once
#include <glm/glm.hpp>

#include "rendering/shadowBuffer.hpp"
#include "rendering/shader.hpp"

struct MeshRenderData {
    glm::mat4 model;
    bool preview = false;
    int shadowMaps = ShadowBuffer::depthMapOffset;

    void uploadToShader(ShaderProgram* shader) const;
};