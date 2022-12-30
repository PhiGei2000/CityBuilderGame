#pragma once
#include "misc/configuration.hpp"

class ShadowBuffer {
  private:
    unsigned int fbo;
    unsigned int depthMaps[Configuration::SHADOW_BUFFER_SPLIT_COUNT];

  public:
    ShadowBuffer();
    ~ShadowBuffer();

    void use(unsigned int mapIndex) const;
    void bindTextures() const;
};
