#pragma once
#include <glm/glm.hpp>

#include <vector>

class InstanceBuffer {
  private:
    unsigned int vbo;

    friend struct Mesh;

  public:
    InstanceBuffer();

    void fillBuffer(const std::vector<glm::vec3>& offsets) const;
};
