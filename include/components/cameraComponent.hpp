/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#include "component.hpp"

#include <glm/glm.hpp>

struct TransformationComponent;

struct CameraComponent : public Component<false> {
    glm::vec3 up, front, right;

    float width = 800.0f;
    float height = 600.0f;
    float fov = 60.0f;
    float near = 0.1f;
    float far = 400.0f;

    float yaw = 0.0f, pitch = 0.0f;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    void calculateMatrices(const TransformationComponent& transform);

    void calculateVectors();
};
