#pragma once
#include <glm/glm.hpp>

#include <vector>

struct TransformationComponent;

class InstanceBuffer {
  private:
    unsigned int vbo;

    friend struct Mesh;

  public:
    InstanceBuffer();

    void fillBuffer(const std::vector<TransformationComponent>& offsets) const;
};
