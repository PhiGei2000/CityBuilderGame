#include "components/lightComponent.hpp"

#include "components/cameraComponent.hpp"
#include "misc/utility.hpp"

#include <glm/gtc/matrix_transform.hpp>

LightComponent::LightComponent(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
    : direction(glm::normalize(direction)), ambient(ambient), diffuse(diffuse), specular(specular) {
}

void LightComponent::assignToEntity(const entt::entity entity, entt::registry& registry) const {
    registry.emplace<LightComponent>(entity, direction, ambient, diffuse, specular);
}

void LightComponent::calculateLightMatrices(const CameraComponent& camera) {
    for (int i = 0; i < Configuration::SHADOW_CASCADE_COUNT; i++) {
        float near = i == 0 ? camera.near : ((camera.far - camera.near) * Configuration::CASCADE_FAR_PLANE_FACTORS[i - 1] + camera.near);
        float far = (camera.far - camera.near) * Configuration::CASCADE_FAR_PLANE_FACTORS[i] + camera.near;

        const auto& [projection, view] = calculateLightMatrices(camera, near, far);
        lightProjection[i] = projection;
        lightView[i] = view;
    }
}

std::vector<glm::vec4> LightComponent::getFrustumInWorldSpace(const glm::mat4& projection, const glm::mat4& view) {
    std::vector<glm::vec4> frustumCorners;
    glm::mat4 inverse = glm::inverse(projection * view);

    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 2; y++) {
            for (int z = 0; z < 2; z++) {
                const glm::vec4 pt = inverse * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                frustumCorners.push_back(pt / pt.w);
            }
        }
    }

    return frustumCorners;
}

std::pair<glm::mat4, glm::mat4> LightComponent::calculateLightMatrices(const CameraComponent& camera, float nearPlane, float farPlane) const {
    const glm::mat4 projection = glm::perspective(glm::radians(camera.fov), camera.width / camera.height, nearPlane, farPlane);

    const std::vector<glm::vec4> frustum = getFrustumInWorldSpace(projection, camera.viewMatrix);

    // calculate frustum center
    glm::vec3 center = glm::vec3(0.0f);
    for (const auto& corner : frustum) {
        center += glm::vec3(corner);
    }
    center /= frustum.size();

    // calculate light view matrix
    // const float theta = utility::cartesianToSpherical(direction).y;

    const glm::vec3 lightUp = glm::vec3(0.0f, 1.0f, 0.0f);

    const glm::mat4 lightView = glm::lookAt(center + direction, center, lightUp);

    // calculate light projection matrix
    float minX = FLT_MAX;
    float maxX = -FLT_MAX;
    float minY = FLT_MAX;
    float maxY = -FLT_MAX;
    float minZ = FLT_MAX;
    float maxZ = -FLT_MAX;
    for (const auto& corner : frustum) {
        const glm::vec4 pl = lightView * corner;
        minX = std::min(minX, pl.x / pl.w);
        maxX = std::max(maxX, pl.x / pl.w);
        minY = std::min(minY, pl.y / pl.w);
        maxY = std::max(maxY, pl.y / pl.w);
        minZ = std::min(minZ, pl.z / pl.w);
        maxZ = std::max(maxZ, pl.z / pl.w);
    }

    constexpr float zMult = 5.1f;

    if (minZ < 0) {
        minZ *= zMult;
    }
    else {
        minZ /= zMult;
    }

    if (maxZ < 0) {
        maxZ /= zMult;
    }
    else {
        maxZ *= zMult;
    }

    const glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

    return std::make_pair(lightProjection, lightView);
}
