/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#include "geometryData.hpp"

#include "misc/typedefs.hpp"

#include <GL/glew.h>

struct VertexAttribute {
    int size;
    int type;
    unsigned char normalized;
    int stride;
    const unsigned int pointer;
    unsigned int vbo = 0;
    int divisor = 0;
};

typedef std::vector<VertexAttribute> VertexAttributes;

class Geometry {
  protected:
    unsigned int vbo, vao, ebo;

    unsigned int drawCount;
    int drawMode;

  public:
    Geometry(const VertexAttributes& attributes, int drawMode = GL_TRIANGLES);

    void setVertexAttribute(unsigned int index, const VertexAttribute& attributes) const;
    void bufferData(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, unsigned int usage = GL_STATIC_DRAW);

    virtual void draw() const;

    void bindBuffer() const;
};

class MeshGeometry : public Geometry {
  private:
    bool culling = true;

  public:
    const static VertexAttributes meshVertexAttributes;

    MeshGeometry();
    MeshGeometry(const GeometryData& data, unsigned int usage = GL_STATIC_DRAW);

    void bufferData(const GeometryData& data, unsigned int usage = GL_STATIC_DRAW);
    void bufferSubData(const std::vector<Vertex>& vertices, unsigned int offset);
    void draw() const override;

    void drawInstanced(unsigned int instancesCount) const;
};

using GeometryPtr = ResourcePtr<Geometry>;
