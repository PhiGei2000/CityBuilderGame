#pragma once
#include "entityEvent.hpp"

#include <glm/glm.hpp>

struct ChunkCreatedEvent : public EntityEvent {
    glm::ivec2 chunkPosition;

    inline ChunkCreatedEvent(const entt::entity entity, const glm::ivec2& position)
        : EntityEvent(entity), chunkPosition(position) {
    }
};

struct ChunkDestroyedEvent : public EntityEvent {
    glm::ivec2 chunkPosition;

    inline ChunkDestroyedEvent(const entt::entity entity, const glm::ivec2& position)
        : EntityEvent(entity), chunkPosition(position) {
    }
};
