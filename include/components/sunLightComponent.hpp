#pragma once
#include "lightComponent.hpp"

/// @brief Special form of light component
struct SunLightComponent : public LightComponent {
    /// @brief Angle relative to y axis
    float angle;

    SunLightComponent(float angle, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
        : LightComponent(glm::vec3(glm::cos(angle), glm::sin(angle), 0.0f), ambient, diffuse, specular), angle(angle) {
    }
};
