#pragma once

#include "misc/typedefs.hpp"

#include <string>
#include <glm/glm.hpp>

class Texture {
  protected:
    unsigned int texture;

  public:
    Texture(const std::string& filename);
    Texture(const std::string& filename, int pixelFormat);
    Texture(const glm::vec3& rgb, int width, int height);
    Texture(const glm::vec4& rgba, int width, int height);

    void use(unsigned int texUnit) const;
};

using TexturePtr = ResourcePtr<Texture>;
