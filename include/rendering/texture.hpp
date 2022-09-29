#pragma once

#include "misc/typedefs.hpp"

#include <string>

class Texture {
  private:
    unsigned int texture;

  public:
    Texture(const std::string& filename);
    Texture(const std::string& filename, int pixelFormat);

    void use(unsigned int texUnit) const;
};

using TexturePtr = ResourcePtr<Texture>;
