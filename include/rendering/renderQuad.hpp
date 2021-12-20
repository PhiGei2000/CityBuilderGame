#pragma once

class RenderQuad {
  private:    
    unsigned int vbo, vao;

  public:
    RenderQuad();
    ~RenderQuad();

    void draw(float xMin, float yMin, float width, float height) const;
};