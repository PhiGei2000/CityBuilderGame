#pragma once
#include "component.hpp"

#include <glm/glm.hpp>

struct BuildMarkerComponent : public Component {
    glm::ivec2 position;
    bool visible;

    inline BuildMarkerComponent(const glm::ivec2& position, bool visible = false)
        : position(position), visible(visible) {
    }
};
