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
