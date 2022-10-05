#pragma once
#include "component.hpp"

#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>

struct CarPath {
    std::vector<glm::vec3> pathIn;
    std::vector<glm::vec3> pathOut;
};

struct CarPathComponent : public AssignableComponent {
    std::unordered_map<std::string, CarPath> paths;

    inline CarPathComponent(std::unordered_map<std::string, CarPath>&& paths)
        : paths(paths) {
    }

    inline CarPathComponent(const std::unordered_map<std::string, CarPath>& paths)
        : paths(paths) {
    }

    inline CarPathComponent(std::initializer_list<std::pair<const std::string, CarPath>> paths = {})
        : paths(paths) {
    }

    inline void assignToEntity(const entt::entity entity, entt::registry& registry) const override {
        registry.emplace<CarPathComponent>(entity, paths);
    }
};
