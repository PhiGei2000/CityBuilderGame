#pragma once
#include "misc/configuration.hpp"

class ShadowBuffer {
  private:
    unsigned int fbo;
    unsigned int depthMap;

  public:
    static constexpr unsigned int depthMapOffset = 4;

    ShadowBuffer();
    ~ShadowBuffer();

    void use() const;
    void bindTextures() const;
};
