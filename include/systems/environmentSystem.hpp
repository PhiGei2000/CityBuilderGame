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
#include "systems/system.hpp"

#include <queue>

struct BuildEvent;
struct ChunkCreatedEvent;
struct TransformationComponent;
struct SunLightComponent;
struct TerrainComponent;

class EnvironmentSystem : public System {
  protected:
    virtual void init() override;

    std::queue<entt::entity> entitiesToDestroy;
    std::queue<glm::ivec2> cellsToClear;

    glm::vec3 sunLight[3] = {
        0.5f * glm::vec3(1.0f, 1.0f, 220.0f / 250.0f),
        glm::vec3(1.0f, 1.0f, 220.0f / 250.0f),
        0.8f * glm::vec3(1.0f, 1.0f, 220.0f / 250.0f),
    };

    entt::entity treeEntity;

    void updateDayNightCycle(float dt, TransformationComponent& sunTransform, SunLightComponent& sunLight) const;

  public:
    EnvironmentSystem(Game* game);

    virtual void update(float dt) override;

    void handleBuildEvent(const BuildEvent& e);

    void handleChunkCreatedEvent(const ChunkCreatedEvent& e) const;
};
