#pragma once
#include <glm/glm.hpp>
#include <functional>

template<>
inline constexpr bool std::less<glm::ivec2>::operator()(const glm::ivec2& x, const glm::ivec2& y) const {
    if (x.x != y.x) {
        return x.y < y.y;
    }

    return x.x < x.y;

}

template<>
inline constexpr bool std::less<std::pair<glm::ivec2, glm::ivec2>>::operator()(const std::pair<glm::ivec2, glm::ivec2>& x, const std::pair<glm::ivec2, glm::ivec2>& y) const {
    std::less<glm::ivec2> nodeComparator;

    if (x.first == y.first) {
        return nodeComparator.operator()(x.second, y.second);
    }

    return nodeComparator.operator()(x.first, y.first);
}