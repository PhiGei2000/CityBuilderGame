#pragma once

#include "rendering/geometryData.hpp"

struct Geometry {
    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;

    unsigned int drawCount;

    Geometry();
    void fillBuffers(const GeometryData& data);

    void bind() const;

    void draw() const;
};