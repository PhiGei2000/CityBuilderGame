#pragma once
#include "rendering/shader.hpp"

struct Rectangle;

class GuiShader : public ShaderProgram {
  public:
    inline GuiShader(const std::string& vertexPath, const std::string& fragmentPath)
        : ShaderProgram(vertexPath, fragmentPath) {
    }

    inline void setRectangle(const std::string& name, const Rectangle& rect) {
        setVector2(name + ".position", rect.getPosition());
        setVector2(name + ".size", glm::vec2{rect.width, rect.height});
    }
};