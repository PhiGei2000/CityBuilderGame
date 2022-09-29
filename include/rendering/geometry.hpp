#pragma once
#include "geometryData.hpp"

#include "misc/typedefs.hpp"

#include <GL/glew.h>

struct VertexAttribute {
    int size;
    int type;
    unsigned char normalized;
    int stride;
    const void* pointer;
};

typedef std::vector<VertexAttribute> VertexAttributes;

class Geometry {
  protected:
    unsigned int vbo, vao, ebo;

    unsigned int drawCount;
    int drawMode;

  public:
    Geometry(const VertexAttributes& attributes, int drawMode = GL_TRIANGLES);

    void fillBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    void bind() const;

    void draw() const;
};

class MeshGeometry : public Geometry {
  private:
    const static VertexAttributes meshVertexAttributes;

  public:
    MeshGeometry();
    MeshGeometry(const GeometryData& data);

    void fillBuffers(const GeometryData& data);
};

using GeometryPtr = ResourcePtr<Geometry>;
