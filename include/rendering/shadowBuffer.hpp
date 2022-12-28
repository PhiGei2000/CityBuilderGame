#pragma once

class ShadowBuffer {
  private:
    unsigned int fbo;
    unsigned int depthMap;

  public:
    ShadowBuffer();
    ~ShadowBuffer();

    void use() const;
    void bindTextures() const;
};
