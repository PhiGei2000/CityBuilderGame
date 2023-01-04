#pragma once
#include "misc/configuration.hpp"
#include "rendering/renderQuad.hpp"
#include "rendering/shadowBuffer.hpp"

class Shader;

class ShadowMapRenderer {
  private:
    RenderQuad quad;
    Shader* shadowDebug;

  public:
    ShadowMapRenderer();
    ~ShadowMapRenderer();

    void render(const ShadowBuffer& buffer) const;
};
