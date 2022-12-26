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
    // frustum in clip space
    glm::vec3 frustum[] = {
        glm::vec3(-1, -1, -1),
        glm::vec3(-1, -1, 1),
        glm::vec3(-1, 1, -1),
        glm::vec3(1, -1, -1),
        glm::vec3(-1, 1, 1),
        glm::vec3(1, -1, 1),
        glm::vec3(1, 1, -1),
        glm::vec3(1, 1, 1)};

    glm::vec3 frustumCenter = glm::vec3(0.0f);

    // transform frustum to world space
    const glm::mat4 inverseTransform = glm::inverse(camera.projectionMatrix * camera.viewMatrix);
    for (int i = 0; i < 8; i++) {
        glm::vec4 transformed = inverseTransform * glm::vec4(frustum[i], 1.0f);
        frustum[i] = 1 / transformed.w * glm::vec3(transformed);

        frustumCenter += frustum[i] / 8.0f;
    }

    glm::vec3 lightPos = frustumCenter - (camera.far - camera.near) * direction;

    // transform direction vector to spherical coordinates and calculate up vector
    // (same calculation as in camera component just with differential geometry)
    float theta = glm::acos(direction.y);
    float phi = glm::sign(direction.z) * glm::acos(direction.x / glm::sin(theta));
    const glm::vec3 lightUp = glm::vec3(-glm::sin(phi), 0, glm::cos(phi));

    lightView = glm::lookAt(lightPos, lightPos + direction, lightUp);

    // transform frustum to light space and calculate projection matrix
    float left = FLT_MAX, right = -FLT_MAX, top = -FLT_MAX, bottom = FLT_MAX, near = FLT_MAX, far = -FLT_MAX;
    for (int i = 0; i < 8; i++) {
        glm::vec4 transformed = lightView * glm::vec4(frustum[i], 1.0f);
        frustum[i] = 1 / transformed.w * glm::vec3(transformed);

        if (left > frustum[i].x)
            left = frustum[i].x;
        if (right < frustum[i].x)
            right = frustum[i].x;
        if (bottom > frustum[i].y)
            bottom = frustum[i].y;
        if (top < frustum[i].y)
            top = frustum[i].y;
        if (far < frustum[i].z)
            far = frustum[i].z;
        if (near > frustum[i].z)
            near = frustum[i].z;
    }

    lightProjection = glm::ortho(left, right, bottom, top, 0.0f, far - near);
}
