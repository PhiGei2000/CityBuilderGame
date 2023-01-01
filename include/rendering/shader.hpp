#pragma once
#include "misc/typedefs.hpp"

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class Shader {
  private:
    unsigned int program = 0;
    std::unordered_map<std::string, unsigned int> uniforms;
    static const std::unordered_map<std::string, std::string> defines;

    static std::string getSource(const std::string& filename);
    static unsigned int compileShader(int shaderType, const std::string& filename);

    unsigned int getLocation(const std::string& name);

  public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);

    void use() const;

    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);

    void setVector2(const std::string& name, const glm::vec2& vec);
    void setVector3(const std::string& name, const glm::vec3& vec);
    void setVector4(const std::string& name, const glm::vec4& vec);

    void setMatrix3(const std::string& name, const glm::mat3& mat);
    void setMatrix4(const std::string& name, const glm::mat4& mat);
};

using ShaderPtr = ResourcePtr<Shader>;
