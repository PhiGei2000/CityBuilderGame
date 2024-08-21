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

InstanceBuffer::InstanceBuffer() {
    glGenBuffers(1, &vbo);
}

unsigned int InstanceBuffer::getVBO() const {
    return vbo;
}

unsigned int InstanceBuffer::getInstancesCount() const {
    return instancesCount;
}

void InstanceBuffer::clearBuffer() {
    instancesCount = 0;

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
