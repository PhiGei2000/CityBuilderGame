#pragma once

#include "rendering/geometryData.hpp"

namespace trafficSimulation {
    struct Geometry {
        unsigned int vbo;
        unsigned int vao;
        unsigned int ebo;

        unsigned int drawCount;

        Geometry();
        void fillBuffers(GeometryData data);

        void bind() const;

        void draw() const;
    };
} // namespace trafficSimulation