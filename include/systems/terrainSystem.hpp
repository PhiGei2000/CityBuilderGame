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
#pragma once
#include "system.hpp"

#include "misc/quadtree.hpp"
#include "misc/terrainArea.hpp"

#include <future>
#include <queue>
#include <vector>

#include <glm/glm.hpp>
#include <noise/noise.h>

struct TerrainComponent;
struct MeshComponent;
struct BuildEvent;
struct TextureAtlas;
struct Vertex;
struct GeometryData;
struct TerrainData;

class TerrainSystem : public System {
  protected:
    noise::module::Const terrainHeightScaleNoise;
    noise::module::Perlin terrainBaseNoise;
    noise::module::ScaleBias terrainRangeNoise;
    noise::module::Floor terrainNoiseFloorModule;
    noise::module::Multiply terrainNoise;
    static constexpr float noiseScaleFactor = 15.0f;

    static const TextureAtlas atlas;

    glm::ivec2 lastChunk = glm::ivec2(-INT_MAX);

    std::queue<glm::ivec2> chunksToGenerate;
    std::queue<glm::ivec2> chunksToCreateMesh;
    std::queue<glm::ivec2> chunksToUpdateMesh;

    static constexpr unsigned int maxThreads = 5;
    std::vector<std::pair<glm::ivec2, std::future<std::pair<GeometryData, GeometryData>>>> meshCreationTasks;
    std::vector<std::pair<glm::ivec2, std::future<std::pair<GeometryData, GeometryData>>>> meshUpdateTasks;

    void init() override;

    float getTerrainHeight(const glm::ivec2& pos) const;
    Quadtree<TerrainData> generateTerrain(const glm::ivec2& chunkPosition) const;

    static std::pair<GeometryData, GeometryData> generateTerrainMesh(const glm::ivec2& chunkPosition, const Quadtree<TerrainData>& terrainData);

    static std::pair<unsigned int, unsigned int> generateTerrainQuadMesh(const glm::ivec2& chunkPosition, std::vector<Vertex>& terrainVertices, std::vector<Vertex>& waterVertices, const Quadtree<TerrainData>& terrainData);
    static unsigned int generateQuad(const glm::vec2& position, float size, const float heights[4], unsigned int textureID, std::vector<Vertex>& vertices);

  public:
    TerrainSystem(Game* game);

    void update(float dt) override;

    // void handleBuildEvent(const BuildEvent& event);
};
