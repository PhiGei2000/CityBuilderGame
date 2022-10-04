#pragma once
#include "component.hpp"
#include "misc/roads/roadGraph.hpp"

#include <glm/gtx/hash.hpp>
#include <map>
#include <unordered_map>

struct RoadComponent : public AssignableComponent {
    RoadGraph graph;

    inline RoadComponent(const RoadGraph& graph)
        : graph(graph) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<RoadComponent>(entity, graph);
    }
};
