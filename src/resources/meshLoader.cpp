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

MeshLoader::FaceData MeshLoader::parseFaceData(std::stringstream& s, const VertexIndices& indexOffsets) {
    FaceData data;
    std::string prefix;

    char nextChar;

    unsigned int posIndex;
    unsigned int texIndex;
    unsigned int normIndex;

    std::string materialName;
    bool culling = true;

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

                indices[i] = VertexIndices(posIndex, texIndex, normIndex) - indexOffsets;
            }

            if (culling) {
                data[materialName].indicesCulling.push_back(indices);
            }
            else {
                data[materialName].indicesNonCulling.push_back(indices);
            }
        }
        else if (prefix == "c") {
            std::string value;
            line >> value;

            if (value == "on") {
                culling = true;
            }
            else if (value == "off") {
                culling = false;
            }
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

void MeshLoader::correctWindingOrder(const VertexData& vertices, FaceData& faces) {
    for (auto& face : faces) {
        for (auto& indices : face.second.indicesCulling) {
            const glm::vec3& p0 = vertices.positions[indices[0].positionIndex];
            const glm::vec3& p1 = vertices.positions[indices[1].positionIndex];
            const glm::vec3& p2 = vertices.positions[indices[2].positionIndex];

            const glm::vec3& normal = vertices.normals[indices[0].normalIndex];
            const glm::vec3 calculatedNormal = glm::cross(p1 - p0, p2 - p0);

            if (glm::dot(calculatedNormal, normal) < 0) {
                // if face is not counterclockwise winding order, swap two indices
                VertexIndices tmp = indices[0];
                indices[0] = indices[1];
                indices[1] = tmp;
            }
        }
    }
}

GeometryData MeshLoader::processFaces(const std::vector<FaceIndices>& indices, const VertexData& vertData) {
    bool vertexExists = false;
    std::vector<VertexIndices> vertices;

    GeometryData data;

    for (const auto& faces : indices) {
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
        data.vertices.emplace_back(
            vertData.positions[indices.positionIndex],
            vertData.texCoords[indices.texCoordIndex],
            vertData.normals[indices.normalIndex]);
    }

    // calculate tangent space
    for (int i = 0; i < data.indices.size(); i += 3) {
        GeometryData::calculateTangentSpace(
            data.vertices[data.indices[i]],
            data.vertices[data.indices[i + 1]],
            data.vertices[data.indices[i + 2]]);
    }

    return data;
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
        glm::vec3 ambientColor = glm::vec3(1.0f);
        glm::vec3 diffuseColor = glm::vec3(1.0f);
        glm::vec3 specularColor = glm::vec3(1.0f);

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

                ambientColor = glm::vec3(r, g, b);
            }
            // diffuse color
            else if (prefix == "Kd") {
                sline >> r;
                sline >> g;
                sline >> b;

                diffuseColor = glm::vec3(r, g, b);
            }
            // specular color
            else if (prefix == "Ks") {
                sline >> r;
                sline >> g;
                sline >> b;

                specularColor = glm::vec3(r, g, b);
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
            // else if (prefix == "illum") {
            //     int value;
            //     sline >> value;

            //     if (value < 0 && value > 2) {
            //         std::cerr << "Failed to load material \"" << materialName << "\". Illumination model " << value << " not supported";

            //         throw std::invalid_argument("Illumination model value out of range");
            //     }

            //     mtl->illuminationModel = value;
            // }
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
            // specular map
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

        // replace missing textures with colors
        if (mtl->ambientTexture == nullptr) {
            mtl->ambientTexture = TexturePtr(new Texture(ambientColor, 1, 1));
        }

        if (mtl->diffuseTexture == nullptr) {
            mtl->diffuseTexture = TexturePtr(new Texture(diffuseColor, 1, 1));
        }

        if (mtl->specularTexture == nullptr) {
            mtl->specularTexture = TexturePtr(new Texture(specularColor, 1, 1));
        }


        materials.emplace(materialName, mtl);

        return materials;
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR:MATERIAL:FILE_NOT_SUCCESSFULLY_READ" << std::endl;

        throw e;
    }
}

MeshPtr MeshLoader::loadMesh(const std::string& filename) {
    Mesh<>* mesh = new Mesh<>();

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

        VertexIndices indexOffsets = VertexIndices(0, 0, 0);

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
                FaceData faceData = parseFaceData(ss, indexOffsets);

                correctWindingOrder(vertData, faceData);

                // process faces
                for (const auto& [materialName, faceIndices] : faceData) {
                    MaterialPtr material = materials.at(materialName);

                    // culled faces
                    GeometryData dataCulling = processFaces(faceIndices.indicesCulling, vertData);
                    dataCulling.culling = true;

                    GeometryPtr cullingGeometry = GeometryPtr(new MeshGeometry(dataCulling));
                    mesh->geometries[objectName].push_back(std::make_pair(material, cullingGeometry));

                    // non culled faces
                    GeometryData dataNonCulling = processFaces(faceIndices.indicesNonCulling, vertData);
                    dataNonCulling.culling = false;

                    GeometryPtr nonCullingGeometry = GeometryPtr(new MeshGeometry(dataNonCulling));
                    mesh->geometries[objectName].push_back(std::make_pair(material, nonCullingGeometry));
                }

                indexOffsets += VertexIndices(vertData.positions.size(), vertData.texCoords.size(), vertData.normals.size());
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
