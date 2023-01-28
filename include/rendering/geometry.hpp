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

    void setVertexAttribute(unsigned int index, const VertexAttribute& attributes, unsigned int vbo = 0, unsigned int divisor = 0) const;
    void fillBuffers(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    virtual void draw() const;
};

class MeshGeometry : public Geometry {
  private:
    bool culling = true;

  public:
    const static VertexAttributes meshVertexAttributes;

    MeshGeometry();
    MeshGeometry(const GeometryData& data);

    void fillBuffers(const GeometryData& data);
    void draw() const override;

    void drawInstanced(unsigned int instancesCount) const;
};

using GeometryPtr = ResourcePtr<Geometry>;
