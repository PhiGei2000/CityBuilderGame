#include "systems/terrainSystem.hpp"

#include "components/components.hpp"
#include "events/buildEvent.hpp"
#include "events/chunkEvents.hpp"
#include "misc/coordinateTransform.hpp"
#include "misc/terrainArea.hpp"
#include "rendering/textureAtlas.hpp"
#include "resources/meshLoader.hpp"

#include <format>

const TextureAtlas TerrainSystem::atlas = TextureAtlas(64.0f, 128.0f, 2, 1);

void TerrainSystem::init() {
    terrainNoise.SetFrequency(0.0001f);
    terrainNoise.SetOctaveCount(4);
    terrainNoise.SetLacunarity(1.9);
}

void TerrainSystem::generateTerrain(TerrainComponent& terrain, const glm::ivec2& chunkPosition) const {
    terrain.heightValues = new float*[Configuration::cellsPerChunk];
    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        terrain.heightValues[x] = new float[Configuration::cellsPerChunk];

        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            glm::vec2 pos = noiseScaleFactor * (static_cast<float>(Configuration::cellSize) * glm::vec2(x, y) + static_cast<float>(Configuration::chunkSize) * glm::vec2(chunkPosition));
            float noiseValue = terrainNoise.GetValue(pos.x, pos.y, 0);
            terrain.heightValues[x][y] = glm::floor(noiseValue * 4 + 2) * 2;
        }
    }
}

unsigned int TerrainSystem::generateTerrainQuadMesh(const glm::ivec2& position, const glm::ivec2& chunkPosition, std::vector<Vertex>& terrainVertices) const {
    int x = position.x, y = position.y;
    TerrainSurfaceTypes surfaceType = game->terrain.getSurfaceType(utility::normalizedChunkGridToNormalizedWorldGridCoords(chunkPosition, position));
    const std::array<glm::vec2, 4>& texCoords = atlas.getQuatTextureCoords(surfaceType == TerrainSurfaceTypes::GRASS ? 1 : 0, 0);
    const glm::ivec2& chunkOffset = Configuration::cellsPerChunk * chunkPosition;

    // generate corners of the quad
    glm::vec3 p0 = glm::vec3(x * Configuration::cellSize, game->terrain.getTerrainHeight(chunkOffset + glm::ivec2(x, y)), y * Configuration::cellSize);
    glm::vec3 p1 = glm::vec3((x + 1) * Configuration::cellSize, game->terrain.getTerrainHeight(chunkOffset + glm::ivec2(x + 1, y)), y * Configuration::cellSize);
    glm::vec3 p2 = glm::vec3(x * Configuration::cellSize, game->terrain.getTerrainHeight(chunkOffset + glm::ivec2(x, y + 1)), (y + 1) * Configuration::cellSize);
    glm::vec3 p3 = glm::vec3((x + 1) * Configuration::cellSize, game->terrain.getTerrainHeight(chunkOffset + glm::ivec2(x + 1, y + 1)), (y + 1) * Configuration::cellSize);

    // calculate tangent space
    const auto& [t1, b1, n1] = Vertex::calculateTangentSpace(p0, p1, p3, texCoords[0], texCoords[1], texCoords[3]);
    const auto& [t2, b2, n2] = Vertex::calculateTangentSpace(p0, p3, p2, texCoords[0], texCoords[3], texCoords[2]);

    // build triangles (maybe not finished)
    terrainVertices.emplace_back(p0, texCoords[0], n1, t1, b1);
    terrainVertices.emplace_back(p1, texCoords[1], n1, t1, b1);
    terrainVertices.emplace_back(p3, texCoords[3], n1, t1, b1);
    terrainVertices.emplace_back(p0, texCoords[0], n2, t2, b2);
    terrainVertices.emplace_back(p3, texCoords[3], n2, t2, b2);
    terrainVertices.emplace_back(p2, texCoords[2], n2, t2, b2);

    return 6;
}

unsigned int TerrainSystem::generateWaterQuadMesh(const glm::ivec2& position, const glm::ivec2& chunkPosition, std::vector<Vertex>& waterVertices) const {
    const glm::ivec2& gridPos = utility::normalizedChunkGridToNormalizedWorldGridCoords(chunkPosition, position);
    if (game->terrain.getSurfaceType(gridPos) != TerrainSurfaceTypes::WATER) {
        return 0;
    }

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

void TerrainSystem::generateTerrainMesh(const glm::ivec2& chunkPosition, MeshComponent& mesh) const {
    std::vector<Vertex> terrainVertices;
    std::vector<unsigned int> terrainIndices;
    unsigned int currentTerrainIndex = 0;

    std::vector<Vertex> waterVertices;
    std::vector<unsigned int> waterIndices;
    unsigned int currentWaterIndex = 0;

    unsigned int terrainIndicesCount, waterIndicesCount;
    for (int x = 0; x < Configuration::cellsPerChunk; x++) {
        for (int y = 0; y < Configuration::cellsPerChunk; y++) {
            terrainIndicesCount = generateTerrainQuadMesh(glm::ivec2(x, y), chunkPosition, terrainVertices);
            for (int i = 0; i < terrainIndicesCount; i++) {
                terrainIndices.push_back(currentTerrainIndex++);
            }

            waterIndicesCount = generateWaterQuadMesh(glm::ivec2(x, y), chunkPosition, waterVertices);
            for (int i = 0; i < waterIndicesCount; i++) {
                waterIndices.push_back(currentWaterIndex++);
            }
        }
    }

    MaterialPtr groundMaterial = resourceManager.getResource<Material>("GROUND_MATERIAL");
    GeometryData terrainData(terrainVertices, terrainIndices, false);
    GeometryPtr terrainGeometry(new MeshGeometry(terrainData, GL_DYNAMIC_DRAW));
    mesh.mesh->geometries["ground"].push_back(std::make_pair(groundMaterial, terrainGeometry));

    if (waterVertices.size() > 0) {
        MaterialPtr waterMaterial = resourceManager.getResource<Material>("WATER_MATERIAL");
        GeometryData waterData(waterVertices, waterIndices, false);
        GeometryPtr waterGeometry(new MeshGeometry(waterData, GL_DYNAMIC_DRAW));
        mesh.mesh->geometries["water"].push_back(std::make_pair(waterMaterial, waterGeometry));
    }
}

void TerrainSystem::updateTerrainMesh(const TerrainArea& area) const {
    const std::unordered_map<glm::ivec2, TerrainArea>& chunkAreas = area.getChunkAreas();

    for (const auto& [chunkPos, chunkArea] : chunkAreas) {
        auto mesh = registry.get<MeshComponent>(game->terrain.chunkEntities[chunkPos]);

        updateTerrainMesh(chunkArea, mesh);
    }
}

void TerrainSystem::updateTerrainMesh(const TerrainArea& area, MeshComponent& mesh) const {
    const auto& [chunkPos, pos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(area.position);

    mesh.mesh->geometries.at("ground").front().second->bindBuffer();

    for (int x = 0; x < area.size.x; x++) {
        std::vector<Vertex> terrainVertices;

        for (int y = 0; y < area.size.y; y++) {
            const glm::ivec2& cellPosition = (pos + glm::ivec2(x, y));

            generateTerrainQuadMesh(cellPosition, chunkPos, terrainVertices);
        }

        unsigned int offset = ((area.position.x + x) * Configuration::cellsPerChunk + area.position.y) * 6;
        glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(Vertex), terrainVertices.size() * sizeof(Vertex), terrainVertices.data());
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TerrainSystem::TerrainSystem(Game* game)
    : System(game) {
    game->getEventDispatcher().sink<BuildEvent>().connect<&TerrainSystem::handleBuildEvent>(*this);

    init();
}

void TerrainSystem::update(float dt) {
    // generate terrain
    while (chunksToGenerate.size() > 0) {
        const glm::ivec2 position = chunksToGenerate.front();

        const entt::entity chunkEntity = registry.create();
        registry.emplace<TransformationComponent>(chunkEntity, utility::normalizedChunkGridToWorldCoords(position, glm::vec2(0.0f)), glm::quat(), glm::vec3(1.0f));
        TerrainComponent& terrain = registry.emplace<TerrainComponent>(chunkEntity);

        generateTerrain(terrain, position);
        game->terrain.chunkEntities[position] = chunkEntity;
        chunksToGenerate.pop();
    }

    // create mesh
    while (chunksToCreateMesh.size() > 0) {
        const glm::ivec2 position = chunksToCreateMesh.front();

        const entt::entity chunkEntity = game->terrain.chunkEntities[position];
        TerrainComponent& terrain = registry.get<TerrainComponent>(chunkEntity);

        MeshComponent& mesh = registry.emplace<MeshComponent>(chunkEntity, MeshPtr(new Mesh()));
        generateTerrainMesh(position, mesh);
        terrain.meshGenerated = true;

        game->log(std::format("TERRAIN_SYSTEM: Created chunk at {}, {}", position.x, position.y));

        ChunkCreatedEvent e(chunkEntity, position);
        game->raiseEvent(e);

        chunksToCreateMesh.pop();
    }

    while (areasToUpdateMesh.size() > 0) {
        const TerrainArea area = areasToUpdateMesh.front();

        updateTerrainMesh(area);

        areasToUpdateMesh.pop();
    }

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

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            const glm::ivec2 chunkPos = glm::ivec2(dx, dy) + currentChunk;
            if (game->terrain.chunkEntities.contains(chunkPos)) {
                const TerrainComponent& terrain = registry.get<TerrainComponent>(game->terrain.chunkEntities[chunkPos]);

                if (!terrain.meshGenerated) {
                    chunksToCreateMesh.push(chunkPos);
                }
            }
        }
    }
}

void TerrainSystem::handleBuildEvent(const BuildEvent& event) {
    if (event.action != BuildAction::DEFAULT) {
        return;
    }

    if (event.type == BuildingType::LIFT_TERRAIN || event.type == BuildingType::LOWER_TERRAIN) {
        const auto& [chunkCoords, cellPos] = utility::normalizedWorldGridToNormalizedChunkGridCoords(event.positions[0]);

        const entt::entity terrainEntity = game->terrain.chunkEntities.at(chunkCoords);
        TerrainComponent& terrain = registry.get<TerrainComponent>(terrainEntity);

        switch (event.type) {
            case BuildingType::LIFT_TERRAIN:
                terrain.heightValues[cellPos.x][cellPos.y] += 2;
                break;
            case BuildingType::LOWER_TERRAIN:
                terrain.heightValues[cellPos.x][cellPos.y] -= 2;
                break;
        }

        areasToUpdateMesh.emplace(cellPos - glm::ivec2(1), glm::ivec2(3, 3));
    }
}