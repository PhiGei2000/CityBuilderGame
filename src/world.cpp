#include "world.hpp"

#include "rendering/shader.hpp"
#include "rendering/vertex.hpp"

#include <iostream>

namespace trafficSimulation {
    void World::init() {
        data = new StreetID*[size];
        for (int i = 0; i < size; i++) {
            data[i] = new StreetID[size];
            for (int j = 0; j < size; j++) {
                data[i][j] = StreetID::NO_STREET;
            }
        }

        Shader* streetShader = new Shader("res/shaders/street.vert", "res/shaders/street.frag");
        resourceManager.setResource("StreetShader", streetShader);

        changed = true;
    }

    void World::destroy() {
        for (int i = 0; i < size; i++) {
            delete[] data[i];
        }

        delete[] data;
    }

    StreetID World::createStreet(int x, int y, StreetID type) {
        StreetID currentStreet = data[x][y];
        if (Street::haveCommonConnections(currentStreet, type)) {
            std::cerr << "Streets at x: " << x << ",y: " << y << " have common connections" << std::endl;
            throw 1;
        }

        data[x][y] = Street::merge(currentStreet, type);
        std::cout << "Created street at x: " << x << " y: " << y
                  << " type: " << data[x][y] << std::endl;;

        changed = true;
        return data[x][y];
    }

    GeometryData World::getVertices() {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        vertices.reserve(12 * size * size);
        indices.reserve(30 * size * size);
        unsigned int index = 0;

        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                StreetID street = data[x][y];

                if (street != StreetID::NO_STREET) {
                    // add vertices
                    vertices.emplace_back(x + 0.25f, y + 0.25f); // bottom left
                    vertices.emplace_back(x + 0.75f, y + 0.25f); // bottom right
                    vertices.emplace_back(x + 0.25f, y + 0.75f); // top left
                    vertices.emplace_back(x + 0.75f, y + 0.75f); // top right

                    // add indices
                    indices.push_back(index + 0);
                    indices.push_back(index + 1);
                    indices.push_back(index + 2);
                    indices.push_back(index + 1);
                    indices.push_back(index + 3);
                    indices.push_back(index + 2);

                    unsigned int indexOffset = 4;

                    if (Street::connectedTop(street)) {
                        // add vertices
                        vertices.emplace_back(x + 0.25f, y + 1.0f);
                        vertices.emplace_back(x + 0.75f, y + 1.0f);

                        // add indices
                        indices.push_back(index + 3);
                        indices.push_back(index + indexOffset);
                        indices.push_back(index + 2);
                        indices.push_back(index + 3);
                        indices.push_back(index + indexOffset + 1);
                        indices.push_back(index + indexOffset);

                        indexOffset += 2;
                    }

                    if (Street::connectedLeft(street)) {
                        // add vertices
                        vertices.emplace_back(x, y + 0.75f);
                        vertices.emplace_back(x, y + 0.25f);

                        // add indices
                        indices.push_back(index);
                        indices.push_back(index + indexOffset);
                        indices.push_back(index + indexOffset + 1);
                        indices.push_back(index);
                        indices.push_back(index + 2);
                        indices.push_back(index + indexOffset);

                        indexOffset += 2;
                    }

                    if (Street::connectedBottom(street)) {
                        // add vertices
                        vertices.emplace_back(x + 0.25f, y);
                        vertices.emplace_back(x + 0.75f, y);

                        // add indices
                        indices.push_back(index);
                        indices.push_back(index + indexOffset);
                        indices.push_back(index + indexOffset + 1);
                        indices.push_back(index);
                        indices.push_back(index + indexOffset + 1);
                        indices.push_back(index + 1);

                        indexOffset += 2;
                    }

                    if (Street::connectedRight(street)) {
                        // add vertices
                        vertices.emplace_back(x + 1.0f, y + 0.75f);
                        vertices.emplace_back(x + 1.0f, y + 0.25f);

                        // add indices
                        indices.push_back(index + 1);
                        indices.push_back(index + indexOffset + 1);
                        indices.push_back(index + 3);
                        indices.push_back(index + 3);
                        indices.push_back(index + indexOffset + 1);
                        indices.push_back(index + indexOffset);

                        indexOffset += 2;
                    }

                    index += indexOffset;
                }
            }
        }

        changed = false;

        return GeometryData{vertices, indices};
    }

    bool World::geometryChanged() const {
        return changed;
    }

    const ResourceManager& World::getResourceManager() const {
        return resourceManager;
    }

    Camera& World::getCamera() {
        return camera;
    }

    const Camera& World::getCamera() const {
        return camera;
    }
} // namespace trafficSimulation