#include "components/lightComponent.hpp"

#include "components/cameraComponent.hpp"

#include <glm/gtc/matrix_transform.hpp>

LightComponent::LightComponent(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
    : direction(glm::normalize(direction)), ambient(ambient), diffuse(diffuse), specular(specular) {
}

void LightComponent::assignToEntity(const entt::entity entity, entt::registry& registry) const {
    registry.emplace<LightComponent>(entity, direction, ambient, diffuse, specular);
}

void LightComponent::calculateLightMatrices(const CameraComponent& camera) {
    float lastSplitDistance = 0.0f;
    for (int k = 0; k < Configuration::SHADOW_CASCADE_COUNT; k++) {
        // frustum in clip space
        glm::vec3 frustum[] = {
            // near plane
            glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f),
            glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f),
            glm::vec4(1.0f, -1.0f, -1.0f, 1.0f),
            glm::vec4(1.0f, 1.0f, -1.0f, 1.0f),
            // far plane
            glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f),
            glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
            glm::vec4(1.0f, -1.0f, 1.0f, 1.0f),
            glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        };

        glm::vec3 frustumCenter = glm::vec3(0.0f);

        // transform frustum to world space
        const glm::mat4 inverseTransform = glm::inverse(camera.projectionMatrix * camera.viewMatrix);
        for (int i = 0; i < 8; i++) {
            glm::vec4 transformed = inverseTransform * glm::vec4(frustum[i], 1.0f);
            frustum[i] = 1 / transformed.w * glm::vec3(transformed);

            frustumCenter += frustum[i] / 8.0f;
        }

        float splitDistance = Configuration::CASCADE_FAR_PLANES_FACTORS[k];
        for (int i = 0; i < 4; i++) {
            glm::vec3 distance = frustum[i + 4] - frustum[i];
            frustum[i + 4] = frustum[i] + distance * splitDistance;
            frustum[i] = frustum[i] + distance * lastSplitDistance;
        }
        lastSplitDistance = splitDistance;

        glm::vec3 lightPos = frustumCenter - (camera.far - camera.near) * direction;

        // transform direction vector to spherical coordinates and calculate up vector
        // (same calculation as in camera component just with differential geometry)
        float theta = glm::acos(direction.y);
        float phi = glm::sign(direction.z) * glm::acos(direction.x / glm::sin(theta));
        const glm::vec3 lightUp = glm::vec3(-glm::sin(phi), 0, glm::cos(phi));

        lightView[k] = glm::lookAt(lightPos, lightPos + direction, lightUp);

        // transform frustum to light space and calculate projection matrix
        float left = FLT_MAX, right = -FLT_MAX, top = -FLT_MAX, bottom = FLT_MAX, near = FLT_MAX, far = -FLT_MAX;
        for (int i = 0; i < 8; i++) {
            glm::vec4 transformed = lightView[k] * glm::vec4(frustum[i], 1.0f);
            frustum[i] = 1 / transformed.w * glm::vec3(transformed);

            left = std::min(left, frustum[i].x);
            right = std::max(right, frustum[i].x);
            bottom = std::min(bottom, frustum[i].y);
            top = std::max(top, frustum[i].y);
            near = std::min(near, frustum[i].z);
            far = std::max(far, frustum[i].z);
        }

        lightProjection[k] = glm::ortho(left, right, bottom, top, 0.0f, far - near);
    }
}
