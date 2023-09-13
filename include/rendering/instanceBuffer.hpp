#pragma once
#include <glm/glm.hpp>

#include <vector>

struct TransformationComponent;

class InstanceBuffer {
  private:
    unsigned int vbo;
    unsigned int instancesCount;

    friend struct Mesh;

  public:
    InstanceBuffer();

    void fillBuffer(const std::vector<TransformationComponent>& offsets);
};
