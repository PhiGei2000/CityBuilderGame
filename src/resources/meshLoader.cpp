#include "resources/meshLoader.hpp"

#include "rendering/texture.hpp"
#include "resources/resourceManager.hpp"

#include "misc/utility.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::stringstream MeshLoader::readLine(std::stringstream& ss) {
    std::string line;
    while (line.empty()) {
        if (!std::getline(ss, line)) {
            return std::stringstream("\0");
        }
    }

    return std::stringstream(line);
}

MeshLoader::VertexData MeshLoader::parseVertexData(std::stringstream& s) {
    VertexData data;
    std::string prefix;

    char nextChar = s.peek();
    while (nextChar == 'v' || nextChar == '#') {

        auto line = readLine(s);
        line >> prefix;

        if (prefix == "v") {
            const glm::vec3& position = parseVec3(line);
            data.positions.emplace_back(position);
        }
        else if (prefix == "vt") {
            const glm::vec2& texCoord = parseVec2(line);
            data.texCoords.emplace_back(texCoord);
        }
        else if (prefix == "vn") {
            const glm::vec3& normal = parseVec3(line);
            data.normals.emplace_back(normal);
        }

        nextChar = s.peek();
    }

    return data;
}

MeshLoader::FaceData MeshLoader::parseFaceData(std::stringstream& s) {
    FaceData data;
    std::string prefix;

    char nextChar;

    unsigned int posIndex;
    unsigned int texIndex;
    unsigned int normIndex;

    std::string materialName;

    while ((nextChar = s.peek()) != 'o' && !s.eof()) {
        auto line = readLine(s);
        line >> prefix;

        if (prefix == "usemtl") {
            line >> materialName;
        }
        else if (prefix == "f") {
            FaceIndices indices;
            for (int i = 0; i < 3; i++) {
                std::string posStr, texStr, normStr;

                std::getline(line, posStr, '/');
                std::getline(line, texStr, '/');
                line >> normStr;

                posIndex = std::stoi(posStr) - 1;
                texIndex = std::stoi(texStr) - 1;
                normIndex = std::stoi(normStr) - 1;

                indices[i] = glm::uvec3(posIndex, texIndex, normIndex);
            }

            data[materialName].push_back(indices);
        }
        else if (prefix.empty()) {
            return data;
        }
    }

    return data;
}

glm::vec2 MeshLoader::parseVec2(std::stringstream& s) {
    float x, y;
    s >> x;
    s >> y;

    return glm::vec2(x, y);
}

glm::vec3 MeshLoader::parseVec3(std::stringstream& s) {
    float x, y, z;
    s >> x;
    s >> y;
    s >> z;

    return glm::vec3(x, y, z);
}

TexturePtr MeshLoader::loadTexture(const std::string& filename, int format) {
    Texture* texture = new Texture(filename, format);

    return TexturePtr(texture);
}

std::unordered_map<std::string, MaterialPtr> MeshLoader::loadMaterials(const std::string& filename) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(filename);

        std::stringstream ss;
        ss << file.rdbuf();

        std::unordered_map<std::string, MaterialPtr> materials;

        MaterialPtr mtl;
        std::string prefix;
        std::string materialName;
        float r, g, b;

        for (std::string line; std::getline(ss, line);) {
            if (line.empty())
                continue;

            std::stringstream sline(line);

            sline >> prefix;
            // new material
            if (prefix == "newmtl") {
                if (mtl != nullptr) {
                    if (!mtl->normalMap) {
                        std::cout << "No normal map loaded for: " + filename << " using default normal map" << std::endl;

                        mtl->normalMap = loadTexture("res/textures/default_normal.png", GL_RGB);
                    }

                    materials.emplace(materialName, mtl);
                }

                mtl = MaterialPtr(new Material());
                sline >> materialName;
            }
            // ambient color
            else if (prefix == "Ka") {
                sline >> r;
                sline >> g;
                sline >> b;

                mtl->ambientColor = glm::vec3(r, g, b);
            }
            // diffuse color
            else if (prefix == "Kd") {
                sline >> r;
                sline >> g;
                sline >> b;

                mtl->diffuseColor = glm::vec3(r, g, b);
            }
            // specular color
            else if (prefix == "Ks") {
                sline >> r;
                sline >> g;
                sline >> b;

                mtl->specularColor = glm::vec3(r, g, b);
            }
            // specular highlights aka shininess
            else if (prefix == "Ns") {
                sline >> mtl->shininess;
            }
            // optical density aka index of refraction
            else if (prefix == "Ni") {
                float ior;
                sline >> ior;

                mtl->specularStrength = pow((ior - 1) / (ior + 1), 2) / 0.08f;
            }
            // dissolve
            else if (prefix == "d") {
                sline >> mtl->dissolve;
            }
            // illumination model
            else if (prefix == "illum") {
                int value;
                sline >> value;

                if (value < 0 && value > 2) {
                    std::cerr << "Failed to load material \"" << materialName << "\". Illumination model " << value << " not supported";

                    throw std::invalid_argument("Illumination model value out of range");
                }

                mtl->illuminationModel = value;
            }
            // ambient map
            else if (prefix == "map_Ka") {
                std::string filename;
                sline >> filename;

                mtl->ambientTexture = loadTexture("res/models/" + filename);
            }
            // diffuse map
            else if (prefix == "map_Kd" || prefix == "map_Ke") {
                std::string filename;
                sline >> filename;

                mtl->diffuseTexture = loadTexture("res/models/" + filename);
            }
            else if (prefix == "map_Ks") {
                std::string filename;
                sline >> filename;

                mtl->specularTexture = loadTexture("res/models/" + filename);
            }
            else if (prefix == "map_Bump") {
                std::unordered_map<std::string, std::string> args;

                std::string arg, argValue;
                sline >> arg;
                while (arg.starts_with('-')) {
                    sline >> argValue;

                    args[arg] = argValue;
                    sline >> arg;
                }

                mtl->normalMap = loadTexture("res/models/" + arg, GL_RGB);
            }
        }

        if (!mtl->normalMap) {
            std::cout << "No normal map loaded for: " + filename << " using default normal map" << std::endl;

            mtl->normalMap = loadTexture("res/textures/default_normal.png", GL_RGB);
        }

        materials.emplace(materialName, mtl);

        return materials;
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR:MATERIAL:FILE_NOT_SUCCESSFULLY_READ" << std::endl;

        throw e;
    }
}

MeshPtr MeshLoader::loadMesh(const std::string& filename, ShaderPtr shader) {
    Mesh* mesh = new Mesh(shader);

    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(filename);

        std::stringstream ss;
        ss << file.rdbuf();

        // process lines
        std::string prefix;

        std::unordered_map<std::string, MaterialPtr> materials;
        std::string objectName;

        glm::uvec3 indexOffsets = glm::uvec3(0);

        for (std::string line; std::getline(ss, line);) {
            std::stringstream sLine(line);
            sLine >> prefix;

            // read material lib
            if (prefix == "mtllib") {
                std::string mtlFile;

                sLine >> mtlFile;

                const auto& mtllib = loadMaterials("res/models/" + mtlFile);
                for (const auto& [name, material] : mtllib) {
                    materials.emplace(name, material);
                }
            }
            // parse object data
            else if (prefix == "o") {
                sLine >> objectName;

                VertexData vertData = parseVertexData(ss);
                FaceData faceData = parseFaceData(ss);

                // process faces
                for (const auto& [materialName, faceIndices] : faceData) {
                    MaterialPtr material = materials.at(materialName);
                    std::vector<glm::uvec3> vertices;

                    GeometryData data;

                    for (const auto& faces : faceIndices) {
                        for (int i = 0; i < 3; i++) {
                            const auto& indices = faces[i];
                            bool vertexExists = false;
                            unsigned int verticesCount = vertices.size();

                            for (int j = 0; j < verticesCount; j++) {
                                const auto& vertexIndices = vertices[j];
                                vertexExists = (indices == vertexIndices);

                                if (vertexExists) {
                                    data.indices.push_back(j);
                                    break;
                                }
                            }

                            if (!vertexExists) {
                                vertices.push_back(indices);
                                data.indices.push_back(verticesCount);
                            }
                        }
                    }

                    for (const auto& indices : vertices) {
                        const glm::uvec3& vertexIndices = indices - indexOffsets;

                        data.vertices.emplace_back(vertData.positions[vertexIndices.x], vertData.texCoords[vertexIndices.y], vertData.normals[vertexIndices.z]);
                    }

                    // calculate tangent space
                    for (int i = 0; i < data.indices.size(); i += 3) {
                        GeometryData::calculateTangentSpace(
                            data.vertices[data.indices[i]],
                            data.vertices[data.indices[i + 1]],
                            data.vertices[data.indices[i + 2]]);
                    }

                    indexOffsets += glm::uvec3(vertData.positions.size(), vertData.texCoords.size(), vertData.normals.size());

                    GeometryPtr geometry = GeometryPtr(new MeshGeometry(data));
                    mesh->geometries[objectName].push_back(std::make_pair(material, geometry));
                }
            }
        }

        file.close();
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR:MODEL:FILE_NOT_SUCCESSFULLY_READ" << std::endl;

        throw e;
    }

    return MeshPtr(mesh);
}
