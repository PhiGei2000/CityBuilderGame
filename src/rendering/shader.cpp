#include "rendering/shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

std::string Shader::getSource(const std::string& filename) {
    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(filename);

        std::stringstream ss;
        ss << file.rdbuf();

        file.close();

        return ss.str();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;

        throw e;
    }
}

unsigned int Shader::compileShader(int shaderType, const std::string& filename) {
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

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
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

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) {
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

void Shader::use() const {
    glUseProgram(program);
}

unsigned int Shader::getLocation(const std::string& name) {
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

void Shader::setBool(const std::string& name, bool value) {
    unsigned int location = getLocation(name);
    glUniform1i(location, (int)value);
}

void Shader::setInt(const std::string& name, int value) {
    unsigned int location = getLocation(name);
    glUniform1i(location, value);
}

void Shader::setFloat(const std::string& name, float value) {
    unsigned int location = getLocation(name);
    glUniform1f(location, value);
}

void Shader::setVector2(const std::string& name, const glm::vec2& vec) {
    unsigned int location = getLocation(name);
    glUniform2f(location, vec.x, vec.y);
}

void Shader::setVector3(const std::string& name, const glm::vec3& vec) {
    unsigned int location = getLocation(name);
    glUniform3f(location, vec.x, vec.y, vec.z);
}

void Shader::setVector4(const std::string& name, const glm::vec4& vec) {
    unsigned int location = getLocation(name);
    glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void Shader::setMatrix3(const std::string& name, const glm::mat3& mat) {
    unsigned int location = getLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMatrix4(const std::string& name, const glm::mat4& mat) {
    unsigned int location = getLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}