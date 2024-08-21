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
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

struct TransformationComponent;

class InstanceBuffer {
  private:
    unsigned int vbo;
    unsigned int instancesCount;

  public:
    InstanceBuffer();

    template<typename TData>
    inline void fillBuffer(const std::vector<TData>& offsets) {
        instancesCount = offsets.size();

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, instancesCount * sizeof(TData), offsets.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void clearBuffer();

    unsigned int getVBO() const;
    unsigned int getInstancesCount() const;
};
