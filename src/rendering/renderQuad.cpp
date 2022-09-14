#include "rendering/renderQuad.hpp"

#include <GL/glew.h>

RenderQuad::RenderQuad() {
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

RenderQuad::~RenderQuad() {
}

void RenderQuad::draw(float xMin, float yMin, float width, float height) const {
    glBindVertexArray(vao);

    float vertices[] = {
        xMin, yMin + height, 0.0f, 0.0f,            // bottom left
        xMin + width, yMin, 1.0f, 1.0f,             // top right
        xMin, yMin, 0.0f, 1.0f,                     // top left

        xMin, yMin + height, 0.0f, 0.0f,            // bottom left
        xMin + width, yMin + height, 1.0f, 0.0f,    // bottom right
        xMin + width, yMin, 1.0f, 1.0f};            // top right

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
