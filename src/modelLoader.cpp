#include "modelLoader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace trafficSimulation {

    void ModelLoader::processFaces(const std::vector<glm::vec3>& positions, const std::vector<glm::vec2>& texCoords, const std::vector<glm::vec3>& normals, const std::vector<FaceIndices>& faces, GeometryData& data) {
        data.vertices.reserve(positions.size());
        data.indices.reserve(faces.size() * 3);
    }

    Geometry ModelLoader::load(const std::string& filename) {
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
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR:MODEL::FILE_NOT_SUCCESSFULLY_READ" << std::endl;

            throw e;
        }
    }
} // namespace trafficSimulation