#pragma once
#include "component.hpp"
#include "misc/roads/path.hpp"

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

struct PathComponent : public AssignableComponent {
    std::unordered_map<std::string, Path> paths;

    inline PathComponent(std::unordered_map<std::string, Path>&& paths)
        : paths(paths) {
    }

    inline PathComponent(const std::unordered_map<std::string, Path>& paths)
        : paths(paths) {
    }

    inline PathComponent(std::initializer_list<std::pair<const std::string, Path>> paths = {})
        : paths(paths) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<PathComponent>(entity, paths);
    }
};
