#pragma once

#include <string>

class Texture {
  private:
    unsigned int texture;

  public:
    Texture(const std::string& filename);

    void use(unsigned int texUnit) const;
};
