#pragma once
#include "component.hpp"

#include <glm/glm.hpp>

struct BuildMarkerComponent : public Component<false> {
    /// @brief Current position of the build marker in normalized world grid coordinates
    glm::ivec2 position;
    /// @brief If true the build marker is rendered otherwise it is not rendered
    bool visible;

    inline BuildMarkerComponent(const glm::ivec2& position, bool visible = false)
        : position(position), visible(visible) {
    }
};
