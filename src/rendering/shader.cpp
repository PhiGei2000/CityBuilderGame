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
#include "rendering/shader.hpp"

#include "misc/configuration.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

const std::unordered_map<std::string, std::string> ShaderProgram::defines = {
    std::make_pair<std::string, std::string>("cascadeCount", std::to_string(Configuration::SHADOW_CASCADE_COUNT))};

std::string ShaderProgram::getSource(const std::string& filename) {
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(filename);
        std::string firstLine;
        std::getline(file, firstLine);

        std::stringstream ss;
        ss << firstLine << std::endl;

        for (const auto [name, value] : defines) {
            ss << "#define " << name << " " << value << std::endl;
        }

        ss << file.rdbuf();

        file.close();

        return ss.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;

        throw e;
    }
}

unsigned int ShaderProgram::compileShader(int shaderType, const std::string& filename) {
    unsigned int shader = glCreateShader(shaderType);
    std::string source = getSource(filename);
    const char* sourcePtr = source.c_str();

    glShaderSource(shader, 1, &sourcePtr, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[256];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);

        std::cout << "ERROR::SHADER::";
        if (shaderType == GL_VERTEX_SHADER) {
            std::cout << "VERTEX::COMPILATION_FAILED\n";
        }
        else if (shaderType == GL_FRAGMENT_SHADER) {
            std::cout << "FRAGMENT::COMPILATION_FAILED\n";
        }
        else if (shaderType == GL_GEOMETRY_SHADER) {
            std::cout << "GEOMETRY::COMPILATION_FAILED\n";
        }
        std::cout << infoLog << std::endl;
    }

    return shader;
}

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    program = glCreateProgram();

    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexPath);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentPath);

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);

        std::cout << "ERROR::SHADER::PROGRAM:LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

ShaderProgram::ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
    program = glCreateProgram();

    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexPath);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentPath);
    unsigned int geometry = compileShader(GL_GEOMETRY_SHADER, geometryPath);

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glAttachShader(program, geometry);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);

        std::cout << "ERROR::SHADER::PROGRAM:LINKING_FAILED\n"
                  << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glDeleteShader(geometry);
}

void ShaderProgram::use() const {
    glUseProgram(program);
}

unsigned int ShaderProgram::getLocation(const std::string& name) {
    auto it = uniforms.find(name);
    if (it == uniforms.end()) {
        unsigned int location = glGetUniformLocation(program, name.c_str());
        uniforms.insert(std::make_pair(name, location));

        return location;
    }
    else {
        return it->second;
    }
}

void ShaderProgram::setBool(const std::string& name, bool value) {
    unsigned int location = getLocation(name);
    glUniform1i(location, (int)value);
}

void ShaderProgram::setInt(const std::string& name, int value) {
    unsigned int location = getLocation(name);
    glUniform1i(location, value);
}

void ShaderProgram::setFloat(const std::string& name, float value) {
    unsigned int location = getLocation(name);
    glUniform1f(location, value);
}

void ShaderProgram::setVector2(const std::string& name, const glm::vec2& vec) {
    unsigned int location = getLocation(name);
    glUniform2f(location, vec.x, vec.y);
}

void ShaderProgram::setVector3(const std::string& name, const glm::vec3& vec) {
    unsigned int location = getLocation(name);
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void ShaderProgram::setVector4(const std::string& name, const glm::vec4& vec) {
    unsigned int location = getLocation(name);
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::setMatrix3(const std::string& name, const glm::mat3& mat) {
    unsigned int location = getLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setMatrix4(const std::string& name, const glm::mat4& mat) {
    unsigned int location = getLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}
