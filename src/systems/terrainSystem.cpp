#include "systems/terrainSystem.hpp"

#include "components/components.hpp"
#include "events/chunkEvents.hpp"
#include "resources/meshLoader.hpp"

#include "misc/coordinateTransform.hpp"
#include "rendering/textureAtlas.hpp"

#include <format>

void TerrainSystem::init() {
    terrainNoise.SetFrequency(0.0001f);
    terrainNoise.SetOctaveCount(4);
    terrainNoise.SetLacunarity(1.9);
}

void TerrainSystem::generateTerrain(TerrainComponent& terrain, const glm::ivec2& chunkPosition) const {
    int cellsPerDirection = Configuration::chunkSize / Configuration::cellSize;

    terrain.heightValues = new float*[cellsPerDirection];
    for (int x = 0; x < cellsPerDirection; x++) {
        terrain.heightValues[x] = new float[cellsPerDirection];

        for (int y = 0; y < cellsPerDirection; y++) {
            glm::vec2 pos = noiseScaleFactor * (static_cast<float>(Configuration::cellSize) * glm::vec2(x, y) + static_cast<float>(Configuration::chunkSize) * glm::vec2(chunkPosition));
            float noiseValue = terrainNoise.GetValue(pos.x, pos.y, 0);
            terrain.heightValues[x][y] = glm::floor(noiseValue * 4 + 2) * 2;
        }
    }
}

void TerrainSystem::generateTerrainMesh(const TerrainComponent& terrain, const glm::ivec2& chunkPosition, MeshComponent& mesh) const {
    int cellsPerDirection = Configuration::chunkSize / Configuration::cellSize;
    std::vector<Vertex> terrainVertices;
    std::vector<unsigned int> terrainIndices;
    unsigned int currentTerrainIndex = 0;

    std::vector<Vertex> waterVertices;
    std::vector<unsigned int> waterIndices;
    unsigned int currentWaterIndex = 0;

    // texture coords
    const TextureAtlas atlas(64.0f, 128.0f, 2, 1);
    const glm::ivec2 chunkOffset = Configuration::chunkSize * chunkPosition;

    for (int x = 0; x < cellsPerDirection; x++) {
        for (int y = 0; y < cellsPerDirection; y++) {
            TerrainSurfaceTypes surfaceType = game->terrain.getSurfaceType(utility::chunkGridToGridCoords(Configuration::cellSize * glm::ivec2(x, y), chunkPosition));
            const std::array<glm::vec2, 4>& texCoords = atlas.getQuatTextureCoords(surfaceType == TerrainSurfaceTypes::GRASS ? 1 : 0, 0);

            // generate corners of the quad
            glm::vec3 p0 = glm::vec3(x * Configuration::cellSize, game->terrain.getTerrainHeight(chunkOffset + Configuration::cellSize * glm::ivec2(x, y)), y * Configuration::cellSize);
            glm::vec3 p1 = glm::vec3((x + 1) * Configuration::cellSize, game->terrain.getTerrainHeight(chunkOffset + Configuration::cellSize * glm::ivec2(x + 1, y)), y * Configuration::cellSize);
            glm::vec3 p2 = glm::vec3(x * Configuration::cellSize, game->terrain.getTerrainHeight(chunkOffset + Configuration::cellSize * glm::ivec2(x, y + 1)), (y + 1) * Configuration::cellSize);
            glm::vec3 p3 = glm::vec3((x + 1) * Configuration::cellSize, game->terrain.getTerrainHeight(chunkOffset + Configuration::cellSize * glm::ivec2(x + 1, y + 1)), (y + 1) * Configuration::cellSize);

            // calculate normals
            // glm::vec3 n0 = glm::normalize(glm::cross(p2 - p0, p1 - p0));
            glm::vec3 n1 = glm::normalize(glm::cross(p0 - p1, p3 - p1));
            glm::vec3 n2 = glm::normalize(glm::cross(p3 - p2, p0 - p2));
            // glm::vec3 n3 = glm::normalize(glm::cross(p1 - p3, p2 - p3));

            // build triangles (maybe not finished)
            terrainVertices.emplace_back(p0, texCoords[0], n1);
            terrainVertices.emplace_back(p1, texCoords[1], n1);
            terrainVertices.emplace_back(p3, texCoords[3], n1);

            terrainVertices.emplace_back(p0, texCoords[0], n2);
            terrainVertices.emplace_back(p3, texCoords[3], n2);
            terrainVertices.emplace_back(p2, texCoords[2], n2);

            for (int i = 0; i < 6; i++) {
                terrainIndices.push_back(currentTerrainIndex++);
            }

            // water plane
            if (surfaceType == TerrainSurfaceTypes::WATER) {
                p0 = static_cast<float>(Configuration::cellSize) * glm::vec3(x, -0.2f, y);
                p1 = static_cast<float>(Configuration::cellSize) * glm::vec3(x + 1, -0.2f, y);
                p2 = static_cast<float>(Configuration::cellSize) * glm::vec3(x, -0.2f, y + 1);
                p3 = static_cast<float>(Configuration::cellSize) * glm::vec3(x + 1, -0.2f, y + 1);

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
    // generate terrain
    while (chunksToGenerate.size() > 0) {
        const glm::ivec2 position = chunksToGenerate.front();

        const entt::entity chunkEntity = registry.create();
        registry.emplace<TransformationComponent>(chunkEntity, utility::chunkToWorldCoords(position), glm::quat(), glm::vec3(1.0f));
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

        generateTerrainMesh(terrain, position, mesh);
        chunksToCreateMesh.pop();
        terrain.meshGenerated = true;

        game->log(std::format("TERRAIN_SYSTEM: Created chunk at {}, {}", position.x, position.y));

        ChunkCreatedEvent e(chunkEntity, position);
        game->raiseEvent(e);
    }

    const TransformationComponent& cameraTransform = registry.get<TransformationComponent>(game->camera);
    const glm::ivec2 currentChunk = utility::worldToChunkCoords(cameraTransform.position);

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
