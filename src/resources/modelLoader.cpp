#include "resources/modelLoader.hpp"

#include "resources/resourceManager.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void ModelLoader::processFaces(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texCoords, const std::vector<glm::vec3>& normals, const std::vector<FaceIndices>& faces, GeometryData& data) {
    data.vertices.reserve(positions.size());
    data.indices.reserve(faces.size() * 3);

    std::vector<std::tuple<unsigned int, unsigned int, unsigned int>> vertices;
    std::vector<unsigned int> indices;

    for (const FaceIndices& face : faces) {
        for (const auto [posIndex, texIndex, normIndex] : face) {
            bool vertexExists = false;
            unsigned int index = vertices.size();

            for (int i = 0; i < vertices.size(); i++) {
                const auto [vertPosIndex, vertTexIndex, vertNormIndex] = vertices[i];
                vertexExists = posIndex == vertPosIndex && texIndex == vertTexIndex && normIndex == vertNormIndex;

                if (vertexExists) {
                    indices.push_back(i);
                    break;
                }
            }

            if (!vertexExists) {
                vertices.emplace_back(posIndex, texIndex, normIndex);
                indices.push_back(index);
            }
        }
    }

    data.indices = indices;
    for (const auto [posIndex, texIndex, normIndex] : vertices) {
        data.vertices.emplace_back(positions[posIndex],
                                   texCoords[texIndex],
                                   normals[normIndex]);
    }
}

GeometryData ModelLoader::load(const std::string& filename) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(filename);

        std::stringstream ss;
        ss << file.rdbuf();

        // process lines
        std::string prefix;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::vec3> normals;
        std::vector<FaceIndices> faceIndices;

        for (std::string line; std::getline(ss, line);) {
            std::stringstream sLine(line);
            sLine >> prefix;

            float x, y, z;
            if (prefix == "v") {
                sLine >> x;
                sLine >> y;
                sLine >> z;

                positions.emplace_back(x, y, z);
            }
            else if (prefix == "vt") {
                sLine >> x;
                sLine >> y;

                texCoords.emplace_back(x, y);
            }
            else if (prefix == "vn") {
                sLine >> x;
                sLine >> y;
                sLine >> z;

                normals.emplace_back(x, y, z);
            }
            else if (prefix == "f") {
                FaceIndices indices;
                for (int i = 0; i < 3; i++) {
                    std::string posStr, texStr, normStr;
                    unsigned int posIndex, texIndex, normIndex;
                    std::getline(sLine, posStr, '/');
                    std::getline(sLine, texStr, '/');
                    sLine >> normStr;

                    posIndex = std::stoi(posStr) - 1;
                    texIndex = std::stoi(texStr) - 1;
                    normIndex = std::stoi(normStr) - 1;

                    indices[i] = std::tuple(posIndex, texIndex, normIndex);
                }

                faceIndices.push_back(indices);
            }
        }

        GeometryData data;
        processFaces(positions, texCoords, normals, faceIndices, data);
        file.close();

        return data;
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR:MODEL:FILE_NOT_SUCCESSFULLY_READ" << std::endl;

        throw e;
    }
}

std::unordered_map<std::string, Material*> ModelLoader::loadMaterials(const std::string& filename, const ResourceManager* resourceManager) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        file.open(filename);

        std::stringstream ss;
        ss << file.rdbuf();

        std::unordered_map<std::string, Material*> materials;

        Material* mtl = nullptr;
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
                    materials.emplace(materialName, mtl);
                }

                mtl = new Material();
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
            else if (prefix == "Kd" || prefix == "Ke") {
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

                mtl->ambientTexture = resourceManager->getResource<Texture>(filename);
            }
            // diffuse map
            else if (prefix == "map_Kd" || prefix == "map_Ke") {
                std::string filename;
                sline >> filename;

                mtl->diffuseTexture = resourceManager->getResource<Texture>(filename);
            }
            else if (prefix == "map_Ks") {
                std::string filename;
                sline >> filename;

                mtl->specularTexture = resourceManager->getResource<Texture>(filename);
            }
        }

        materials.emplace(materialName, mtl);

        return materials;
    }
    catch (std::ifstream::failure e) {
        std::cout << "ERROR:MATERIAL:FILE_NOT_SUCCESSFULLY_READ" << std::endl;

        throw e;
    }
}
