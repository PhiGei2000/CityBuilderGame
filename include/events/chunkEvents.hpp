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
#include "entityEvent.hpp"

#include "misc/terrainArea.hpp"

#include <vector>

#include <glm/glm.hpp>

struct ChunkEvent : public EntityEvent {
    glm::ivec2 chunkPosition;

    inline ChunkEvent(const entt::entity entity, const glm::ivec2& position)
        : EntityEvent(entity), chunkPosition(position) {
    }
};

struct ChunkCreatedEvent : public ChunkEvent {

    inline ChunkCreatedEvent(const entt::entity entity, const glm::ivec2& position)
        : ChunkEvent(entity, position) {
    }
};

struct ChunkUpdatedEvent : public ChunkEvent {
    TerrainArea area;

    inline ChunkUpdatedEvent(const entt::entity entity, const glm::ivec2& position, const TerrainArea& area)
        : ChunkEvent(entity, position), area(area) {
    }
};

struct ChunkDestroyedEvent : public ChunkEvent {
    inline ChunkDestroyedEvent(const entt::entity entity, const glm::ivec2& position)
        : ChunkEvent(entity, position) {
    }
};
