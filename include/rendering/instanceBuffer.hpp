#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

struct TransformationComponent;

class InstanceBuffer {
  private:
    unsigned int vbo;
    unsigned int instancesCount;

  public:
    InstanceBuffer();

    template<typename TData>
    inline void fillBuffer(const std::vector<TData>& offsets) {
        instancesCount = offsets.size();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, instancesCount * sizeof(TData), offsets.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void clearBuffer();

    unsigned int getVBO() const;
    unsigned int getInstancesCount() const;
};
