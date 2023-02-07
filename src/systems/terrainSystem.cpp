#include "systems/terrainSystem.hpp"

#include "components/components.hpp"
#include "resources/meshLoader.hpp"

#include "noise/noise.h"

void TerrainSystem::init() {
    TerrainComponent& terrain = registry.emplace<TerrainComponent>(game->terrain);
    generateTerrain(terrain);

    MeshComponent& terrainMesh = registry.emplace<MeshComponent>(game->terrain, MeshPtr(new Mesh()));
    generateTerrainMesh(terrain, terrainMesh);

    registry.emplace<TransformationComponent>(game->terrain, glm::vec3(0.0f), glm::quat(), glm::vec3(1.0f));
}

void TerrainSystem::generateTerrain(TerrainComponent& terrain) const {
    int cellsPerDirection = Configuration::worldSize / Configuration::gridSize;

    noise::module::Perlin perlin;
    perlin.SetFrequency(0.0001f);
    perlin.SetOctaveCount(4);
    perlin.SetLacunarity(1.9);

    terrain.heightValues = new float*[cellsPerDirection + 1];
    for (int x = 0; x < cellsPerDirection + 1; x++) {
        terrain.heightValues[x] = new float[cellsPerDirection + 1];

        for (int y = 0; y < cellsPerDirection + 1; y++) {
            
            float heightValue = glm::floor(perlin.GetValue(x * 100, y * 100, 0) * 4 + 2) * 2;
            terrain.heightValues[x][y] = heightValue;
        }
    }
}

void TerrainSystem::generateTerrainMesh(const TerrainComponent& terrain, MeshComponent& mesh) const {
    int cellsPerDirection = Configuration::worldSize / Configuration::gridSize;
    std::vector<Vertex> terrainVertices;
    std::vector<unsigned int> terrainIndices;
    unsigned int currentTerrainIndex = 0;

    std::vector<Vertex> waterVertices;
    std::vector<unsigned int> waterIndices;
    unsigned int currentWaterIndex = 0;

    // texture coords
    constexpr glm::vec2 t0 = glm::vec2(0.0f, 0.0f);
    constexpr glm::vec2 t1 = glm::vec2(1.0f, 0.0f);
    constexpr glm::vec2 t2 = glm::vec2(0.0f, 1.0f);
    constexpr glm::vec2 t3 = glm::vec2(1.0f, 1.0f);

    for (int x = 0; x < cellsPerDirection - 1; x++) {
        for (int y = 0; y < cellsPerDirection - 1; y++) {
            // generate corners of the quad
            glm::vec3 p0 = glm::vec3(x * Configuration::gridSize, terrain.heightValues[x][y], y * Configuration::gridSize);
            glm::vec3 p1 = glm::vec3((x + 1) * Configuration::gridSize, terrain.heightValues[x + 1][y], y * Configuration::gridSize);
            glm::vec3 p2 = glm::vec3(x * Configuration::gridSize, terrain.heightValues[x][y + 1], (y + 1) * Configuration::gridSize);
            glm::vec3 p3 = glm::vec3((x + 1) * Configuration::gridSize, terrain.heightValues[x + 1][y + 1], (y + 1) * Configuration::gridSize);

            // calculate normals
            glm::vec3 n0 = glm::normalize(glm::cross(p2 - p0, p1 - p0));
            glm::vec3 n1 = glm::normalize(glm::cross(p0 - p1, p3 - p1));
            glm::vec3 n2 = glm::normalize(glm::cross(p3 - p2, p0 - p2));
            glm::vec3 n3 = glm::normalize(glm::cross(p1 - p3, p2 - p3));

            // build triangles (maybe not finished)
            terrainVertices.emplace_back(p0, t0, n1);
            terrainVertices.emplace_back(p1, t1, n1);
            terrainVertices.emplace_back(p3, t3, n1);

            terrainVertices.emplace_back(p0, t0, n2);
            terrainVertices.emplace_back(p3, t3, n2);
            terrainVertices.emplace_back(p2, t2, n2);

            for (int i = 0; i < 6; i++) {
                terrainIndices.push_back(currentTerrainIndex++);
            }

            // water plane
            if (terrain.isWater(static_cast<float>(Configuration::gridSize) * glm::vec2(x + 0.5f, y + 0.5f))) {
                p0 = static_cast<float>(Configuration::gridSize) * glm::vec3(x, -0.2f, y);
                p1 = static_cast<float>(Configuration::gridSize) * glm::vec3(x + 1, -0.2f, y);
                p2 = static_cast<float>(Configuration::gridSize) * glm::vec3(x, -0.2f, y + 1);
                p3 = static_cast<float>(Configuration::gridSize) * glm::vec3(x + 1, -0.2f, y + 1);

                glm::vec3 waterNormal = glm::vec3(0.0f, 1.0f, 0.0f);

                // TODO: optimize this
                // build triangles
                waterVertices.emplace_back(p0, t0, waterNormal);
                waterVertices.emplace_back(p1, t1, waterNormal);
                waterVertices.emplace_back(p3, t3, waterNormal);

                waterVertices.emplace_back(p0, t0, waterNormal);
                waterVertices.emplace_back(p3, t3, waterNormal);
                waterVertices.emplace_back(p2, t2, waterNormal);

                for (int i = 0; i < 6; i++) {
                    waterIndices.push_back(currentWaterIndex++);
                }
            }
        }
    }

    MaterialPtr groundMaterial = resourceManager.getResource<Material>("GROUND_MATERIAL");
    GeometryData terrainData(terrainVertices, terrainIndices, false);
    GeometryPtr terrainGeometry(new MeshGeometry(terrainData));
    mesh.mesh->geometries["ground"].push_back(std::make_pair(groundMaterial, terrainGeometry));

    if (waterVertices.size() > 0) {
        MaterialPtr waterMaterial = resourceManager.getResource<Material>("WATER_MATERIAL");
        GeometryData waterData(waterVertices, waterIndices, false);
        GeometryPtr waterGeometry(new MeshGeometry(waterData));
        mesh.mesh->geometries["water"].push_back(std::make_pair(waterMaterial, waterGeometry));
    }
}

TerrainSystem::TerrainSystem(Game* game)
    : System(game) {
    init();
}

void TerrainSystem::update(float dt) {
}
