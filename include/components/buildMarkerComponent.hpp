#pragma once

#include <glm/glm.hpp>

struct BuildMarkerComponent {
    glm::ivec2 position;
    bool visible;

    inline BuildMarkerComponent(const glm::ivec2& position, bool visible = false)
        : position(position), visible(visible) {
    }
};
