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
    ~Geometry();

    void setVertexAttribute(unsigned int index, const VertexAttribute& attributes) const;

    template<typename T>
    inline void bufferData(const std::vector<T>& vertices, const std::vector<unsigned int>& indices, unsigned int usage = GL_STATIC_DRAW) {
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), usage);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usage);

        if (indices.size() == 0) {
            drawCount = vertices.size();
        }
        else {
            drawCount = indices.size();
        }

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    virtual void drawArrays() const;

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
