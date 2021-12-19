#pragma once
#include "../rendering/shader.hpp"

class RenderQuad {
  private:
    static constexpr float vertices[] = {
        // x, y, tx, ty
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f};

    static constexpr unsigned int indices[] = {0, 1, 2, 0, 3, 1};
    static constexpr unsigned int drawCount = 6;

    unsigned int vbo, vao, ebo;
    Shader* shader;

  public:
    RenderQuad();
    ~RenderQuad();

    void draw() const;

    Shader* getShader() const;
};  