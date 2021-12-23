#pragma once
#include "geometryData.hpp"

class Geometry {
    unsigned int vbo, vao, ebo;

    unsigned int drawCount;

  public:
    Geometry();

    void fillBuffers(const GeometryData& data);

    void bind() const;

    void draw() const;
};
