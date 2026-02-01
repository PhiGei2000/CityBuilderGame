#pragma once

#include <GL/glew.h>

struct Rectangle;

class RenderQuad {
  private:
    unsigned int vbo, vao;

  public:
    RenderQuad();
    ~RenderQuad();

    void draw(float xMin, float yMin, float width, float height, int drawMode = GL_TRIANGLES) const;
    void draw(const Rectangle& rect, int drawMode = GL_TRIANGLES) const;
};
