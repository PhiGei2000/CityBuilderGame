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
#include "systems/terrainSystem.hpp"

#include "components/components.hpp"
#include "events/buildEvent.hpp"
#include "events/chunkEvents.hpp"
#include "misc/coordinateTransform.hpp"
#include "misc/terrain.hpp"
#include "misc/terrainArea.hpp"
#include "rendering/textureAtlas.hpp"
#include "resources/meshLoader.hpp"

#include <chrono>
#include <format>

const TextureAtlas TerrainSystem::atlas = TextureAtlas(64.0f, 128.0f, 2, 1);

void TerrainSystem::init() {
    using namespace noise::module;

    terrainHeightScaleNoise.SetConstValue(Configuration::Terrain::heightSteps);
    terrainNoise.SetSourceModule(0, terrainHeightScaleNoise);

    terrainBaseNoise.SetFrequency(0.0001f);
    terrainBaseNoise.SetOctaveCount(4);
    terrainBaseNoise.SetLacunarity(1.9);

    static constexpr float terrainNoiseScale = Configuration::Terrain::heightRange / 2;
    terrainRangeNoise.SetBias(Configuration::Terrain::minHeight + terrainNoiseScale);
    terrainRangeNoise.SetScale(terrainNoiseScale);
    terrainRangeNoise.SetSourceModule(0, terrainBaseNoise);

    terrainNoiseFloorModule.SetSourceModule(0, terrainRangeNoise);

    terrainNoise.SetSourceModule(1, terrainNoiseFloorModule);
}

float TerrainSystem::getTerrainHeight(const glm::ivec2& position) const {
    // glm::vec2 pos = noiseScaleFactor * glm::vec2(position);
    //  noiseValue in [-1,1]
    //  float noiseValue = terrainNoise.GetValue(pos.x, pos.y, 0);
    // TODO: Use noise
    return 0.0f;
}

TerrainSystem::TerrainCreationData TerrainSystem::generateTerrain(const glm::ivec2& chunkPosition) const {
    TerrainSystem::TerrainCreationData data{
        new float*[Configuration::cellsPerChunk + 1],
        new TerrainSurfaceTypes*[Configuration::cellsPerChunk]};

    constexpr glm::ivec2 offsets[9] = {
        glm::ivec2(0, 0),
        glm::ivec2(1, 0),
        glm::ivec2(0, 1),
        glm::ivec2(1, 1),
        glm::ivec2(-1, 0),
        glm::ivec2(0, -1),
        glm::ivec2(-1, -1),
        glm::ivec2(-1, 1),
        glm::ivec2(1, -1),
    };

    for (int x = 0; x < Configuration::cellsPerChunk + 1; x++) {
        data.heightValues[x] = new float[Configuration::cellsPerChunk + 1];
        if (x < Configuration::cellsPerChunk) {
            data.surfaceTypes[x] = new TerrainSurfaceTypes[Configuration::cellsPerChunk];
        }

        for (int y = 0; y < Configuration::cellsPerChunk + 1; y++) {

            const glm::ivec2 position = glm::ivec2(x, y) + Configuration::chunkSize * chunkPosition;
            data.heightValues[x][y] = getTerrainHeight(position);

            if (x < Configuration::cellsPerChunk && y < Configuration::cellsPerChunk) {
                for (int i = 0; i < 4; i++) {
                    if (getTerrainHeight(position + offsets[i]) < 0.0f) {
                        data.surfaceTypes[x][y] = TerrainSurfaceTypes::WATER;
                    }
                }

                int waterCount = 0;
                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 4; j++) {
                        if (getTerrainHeight(position + offsets[j] + offsets[i]) < 0) {
                            waterCount++;
                            j = 4;
                        }
                    }
                }

                if (waterCount >= 1) {
                    data.surfaceTypes[x][y] = TerrainSurfaceTypes::BEACH;
                }

                data.surfaceTypes[x][y] = TerrainSurfaceTypes::GRASS;
            }
        }
    }

    return data;
}

unsigned int TerrainSystem::generateTerrainQuadMesh(const glm::ivec2& position, const glm::ivec2& chunkPosition, std::vector<Vertex>& terrainVertices, float** const heightValues, TerrainSurfaceTypes surfaceType) {
    int x = position.x, y = position.y;
    const std::array<glm::vec2, 4>& texCoords = atlas.getQuatTextureCoords(surfaceType == TerrainSurfaceTypes::GRASS ? 1 : 0, 0);
    const glm::ivec2& chunkOffset = Configuration::cellsPerChunk * chunkPosition;

    // generate corners of the quad
    glm::vec3 positions[4] = {
        glm::vec3(x * Configuration::cellSize, heightValues[x][y], y * Configuration::cellSize),
        glm::vec3((x + 1) * Configuration::cellSize, heightValues[x + 1][y], y * Configuration::cellSize),
        glm::vec3(x * Configuration::cellSize, heightValues[x][y + 1], (y + 1) * Configuration::cellSize),
        glm::vec3((x + 1) * Configuration::cellSize, heightValues[x + 1][y + 1], (y + 1) * Configuration::cellSize),
    };

    std::array<unsigned int, 3> triangle1, triangle2;
    if (glm::length(positions[0] - positions[3]) < glm::length(positions[1] - positions[2])) {
        triangle1 = {0, 1, 3};
        triangle2 = {0, 3, 2};
    }
    else {
        triangle1 = {0, 1, 2};
        triangle2 = {1, 3, 2};
    }

    // calculate tangent space
    const auto& [t1, b1, n1] = Vertex::calculateTangentSpace(positions[triangle1[0]], positions[triangle1[1]], positions[triangle1[2]], texCoords[triangle1[0]], texCoords[triangle1[1]], texCoords[triangle1[2]]);
    const auto& [t2, b2, n2] = Vertex::calculateTangentSpace(positions[triangle2[0]], positions[triangle2[1]], positions[triangle2[2]], texCoords[triangle2[0]], texCoords[triangle2[1]], texCoords[triangle2[2]]);

    // build triangles (maybe not finished)
    terrainVertices.emplace_back(positions[triangle1[0]], texCoords[triangle1[0]], n1, t1, b1);
    terrainVertices.emplace_back(positions[triangle1[1]], texCoords[triangle1[1]], n1, t1, b1);
    terrainVertices.emplace_back(positions[triangle1[2]], texCoords[triangle1[2]], n1, t1, b1);
    terrainVertices.emplace_back(positions[triangle2[0]], texCoords[triangle2[0]], n2, t2, b2);
    terrainVertices.emplace_back(positions[triangle2[1]], texCoords[triangle2[1]], n2, t2, b2);
    terrainVertices.emplace_back(positions[triangle2[2]], texCoords[triangle2[2]], n2, t2, b2);

    return 6;
}

unsigned int TerrainSystem::generateWaterQuadMesh(const glm::ivec2& position, const glm::ivec2& chunkPosition, std::vector<Vertex>& waterVertices) {
    const glm::ivec2& gridPos = utility::normalizedChunkGridToNormalizedWorldGridCoords(chunkPosition, position);
    float x = position.x, y = position.y;

    glm::vec3 p0 = static_cast<float>(Configuration::cellSize) * glm::vec3(x, -0.2f, y);
    glm::vec3 p1 = static_cast<float>(Configuration::cellSize) * glm::vec3(x + 1, -0.2f, y);
    glm::vec3 p2 = static_cast<float>(Configuration::cellSize) * glm::vec3(x, -0.2f, y + 1);
    glm::vec3 p3 = static_cast<float>(Configuration::cellSize) * glm::vec3(x + 1, -0.2f, y + 1);

    constexpr glm::vec3 waterNormal = glm::vec3(0.0f, 1.0f, 0.0f);
    constexpr glm::vec2 t0 = glm::vec2(0.0f, 0.0f);
    constexpr glm::vec2 t1 = glm::vec2(1.0f, 0.0f);
    constexpr glm::vec2 t2 = glm::vec2(0.0f, 1.0f);
    constexpr glm::vec2 t3 = glm::vec2(1.0f, 1.0f);

    // TODO: optimize this
    // build triangles
    waterVertices.emplace_back(p0, t0, waterNormal);
    waterVertices.emplace_back(p1, t1, waterNormal);
    waterVertices.emplace_back(p3, t3, waterNormal);

    waterVertices.emplace_back(p0, t0, waterNormal);
    waterVertices.emplace_back(p3, t3, waterNormal);
    waterVertices.emplace_back(p2, t2, waterNormal);

    return 6;
}

std::pair<GeometryData, GeometryData> TerrainSystem::generateTerrainMesh(const glm::ivec2& chunkPosition, float** const heightMap, TerrainSurfaceTypes** const surfaceTypes) {
    std::vector<Vertex> terrainVertices;
    std::vector<unsigned int> terrainIndices;
    unsigned int currentTerrainIndex = 0;

    std::vector<Vertex> waterVertices;
    std::vector<unsigned int> waterIndices;
    unsigned int currentWaterIndex = 0;

    unsigned int terrainIndicesCount, waterIndicesCount;
    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            terrainIndicesCount = generateTerrainQuadMesh(glm::ivec2(x, y), chunkPosition, terrainVertices, heightMap, surfaceTypes[x][y]);
            for (int i = 0; i < terrainIndicesCount; i++) {
                terrainIndices.push_back(currentTerrainIndex++);
            }

            if (surfaceTypes[x][y] == TerrainSurfaceTypes::WATER) {
                waterIndicesCount = generateWaterQuadMesh(glm::ivec2(x, y), chunkPosition, waterVertices);
                for (int i = 0; i < waterIndicesCount; i++) {
                    waterIndices.push_back(currentWaterIndex++);
                }
            }
        }
    }

    GeometryData terrainData(terrainVertices, terrainIndices, false);
    GeometryData waterData(waterVertices, waterIndices, false);

    return std::make_pair(terrainData, waterData);
}

void TerrainSystem::updateTerrainMesh(const TerrainArea& area) const {
    const std::unordered_map<glm::ivec2, TerrainArea>& chunkAreas = area.getChunkAreas();

    for (const auto& [chunkPos, chunkArea] : chunkAreas) {
        auto mesh = registry.get<MeshComponent>(game->terrain.chunkEntities[chunkPos]);

        updateTerrainMesh(chunkArea, mesh);
    }
}

void TerrainSystem::updateTerrainMesh(const TerrainArea& area, MeshComponent& mesh) const {
    // const auto& [chunkPos, pos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(area.position);

    // mesh.mesh->geometries.at("ground").front().second->bindBuffer();

    // for (int x = 0; x < area.size.x; x++) {
    //     std::vector<Vertex> terrainVertices;

    //     for (int y = 0; y < area.size.y; y++) {
    //         const glm::ivec2& cellPosition = (pos + glm::ivec2(x, y));

    //         std::cout << "Update terrain at " << cellPosition << std::endl;
    //         generateTerrainQuadMesh(cellPosition, chunkPos, terrainVertices);
    //     }

    //     unsigned int offset = ((pos.x + x) * Configuration::cellsPerChunk + pos.y) * 6;
    //     glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(Vertex), terrainVertices.size() * sizeof(Vertex), terrainVertices.data());
    // }

    // glBindVertexArray(0);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TerrainSystem::TerrainSystem(Game* game)
    : System(game) {
    // game->getEventDispatcher().sink<BuildEvent>().connect<&TerrainSystem::handleBuildEvent>(*this);

    init();
}

void TerrainSystem::update(float dt) {
    MaterialPtr groundMaterial = resourceManager.getResource<Material>("GROUND_MATERIAL");
    MaterialPtr waterMaterial = resourceManager.getResource<Material>("WATER_MATERIAL");
    ShaderPtr meshShader = resourceManager.getResource<Shader>("MESH_SHADER");

    for (auto it = meshCreationTasks.begin(); it != meshCreationTasks.end();) {
        if (it->second.valid() && it->second.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            const glm::ivec2& chunkPos = it->first;
            const auto& [terrainGeometry, waterGeometry] = it->second.get();

            entt::entity chunk = game->terrain.chunkEntities[chunkPos];
            TerrainComponent& terrain = registry.get<TerrainComponent>(chunk);
            MeshComponent& mesh = registry.emplace<MeshComponent>(chunk, MeshPtr(new Mesh()));

            mesh.mesh->geometries["ground"].emplace_back(groundMaterial, new MeshGeometry(terrainGeometry));
            mesh.mesh->geometries["water"].emplace_back(waterMaterial, new MeshGeometry(waterGeometry));
            mesh.mesh->shader = meshShader;
            terrain.meshGenerated = true;

            it = meshCreationTasks.erase(it);
            game->log(std::format("TERRAIN_SYSTEM: Created chunk at {}, {}", chunkPos.x, chunkPos.y));

            ChunkCreatedEvent e(chunk, chunkPos);
            game->raiseEvent(e);
        }
        else {
            it++;
        }
    }

    // generate terrain
    while (chunksToGenerate.size() > 0) {
        const glm::ivec2 position = chunksToGenerate.front();

        // create chunk and assign components
        const entt::entity chunkEntity = registry.create();
        registry.emplace<TransformationComponent>(chunkEntity, utility::normalizedChunkGridToWorldCoords(position, glm::vec2(0.0f)), glm::quat(), glm::vec3(1.0f));
        TerrainComponent& terrain = registry.emplace<TerrainComponent>(chunkEntity);
        registry.emplace<RoadComponent>(chunkEntity);
        registry.emplace<RoadMeshComponent>(chunkEntity);
        game->terrain.chunkEntities[position] = chunkEntity;

        // generate terrain height
        const TerrainCreationData& creationData = generateTerrain(position);
        terrain.heightValues = creationData.heightValues;
        terrain.surfaceTypes = creationData.surfaceTypes;
        chunksToGenerate.pop();

        chunksToCreateMesh.push(position);
    }

    // create mesh
    while (chunksToCreateMesh.size() > 0 && meshCreationTasks.size() < maxThreads) {
        const glm::ivec2 position = chunksToCreateMesh.front();

        const entt::entity chunkEntity = game->terrain.chunkEntities[position];
        const TerrainComponent& terrain = registry.get<TerrainComponent>(chunkEntity);

        meshCreationTasks.emplace_back(position, std::async(
                                                     std::launch::async, [=, this]() {
                                                         return generateTerrainMesh(position, terrain.heightValues, terrain.surfaceTypes);
                                                     }));

        chunksToCreateMesh.pop();
    }

    // while (areasToUpdateMesh.size() > 0) {
    //     const TerrainArea area = areasToUpdateMesh.front();

    //     updateTerrainMesh(area);

    //     areasToUpdateMesh.pop();
    // }

    const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(game->camera);
    const auto [currentChunk, _] = utility::worldToNormalizedChunkGridCoords(cameraTransform.position);

    if (currentChunk != lastChunk) {
        lastChunk = currentChunk;
        // chunks to generate
        for (int dx = -2; dx <= 2; dx++) {
            for (int dy = -2; dy <= 2; dy++) {
                const glm::ivec2 chunkPos = glm::ivec2(dx, dy) + currentChunk;
                // check if chunk already loaded and generate it if not
                if (!game->terrain.chunkEntities.contains(chunkPos)) {
                    chunksToGenerate.push(chunkPos);
                }
            }
        }
    }

    // TODO: Destroy chunks
}

// void TerrainSystem::handleBuildEvent(const BuildEvent& event) {
//     if (event.action != BuildAction::DEFAULT) {
//         return;
//     }
//
//     if (event.type == BuildingType::LIFT_TERRAIN || event.type == BuildingType::LOWER_TERRAIN) {
//         const auto& [chunkCoords, cellPos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(event.positions[0]);
//
//         const entt::entity terrainEntity = game->terrain.chunkEntities.at(chunkCoords);
//         TerrainComponent& terrain = registry.get<TerrainComponent>(terrainEntity);
//
//         switch (event.type) {
//             case BuildingType::LIFT_TERRAIN:
//                 terrain.heightValues[cellPos.x][cellPos.y] += 2;
//                 break;
//             case BuildingType::LOWER_TERRAIN:
//                 terrain.heightValues[cellPos.x][cellPos.y] -= 2;
//                 break;
//         }
//
//         areasToUpdateMesh.emplace(event.positions[0] - glm::ivec2(1), glm::ivec2(3, 3));
//     }
// }