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
#include "rendering/instanceBuffer.hpp"

#include "components/transformationComponent.hpp"
#include "misc/roads/roadTile.hpp"

#include <GL/glew.h>

InstanceBuffer::InstanceBuffer() {
    glGenBuffers(1, &vbo);
}

template<>
void InstanceBuffer::fillBuffer(const std::vector<TransformationComponent>& transformations) {
    std::vector<glm::mat4> matrices;
    matrices.reserve(transformations.size());

    for (const auto& transform : transformations) {
        matrices.push_back(transform.transform);
    }
    instancesCount = matrices.size();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, instancesCount * sizeof(glm::mat4), &matrices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<>
void InstanceBuffer::fillBuffer(const std::vector<RoadRenderData>& transformations) {
    instancesCount = transformations.size();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, instancesCount * sizeof(RoadRenderData), &transformations[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
