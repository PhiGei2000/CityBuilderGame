#pragma once
#include "misc/configuration.hpp"
#include "rendering/renderQuad.hpp"
#include "rendering/shadowBuffer.hpp"

class ShaderProgram;

class ShadowMapRenderer {
  private:
    RenderQuad quad;
    ShaderProgram* shadowDebug;

  public:
    ShadowMapRenderer();
    ~ShadowMapRenderer();

    void render(const ShadowBuffer& buffer) const;
};
