/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
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
