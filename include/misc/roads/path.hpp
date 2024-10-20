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
#include <queue>
#include <vector>

#include <glm/glm.hpp>

struct Path {
  protected:
    std::vector<glm::vec3> positions;

  public:
    Path();
    Path(const std::initializer_list<glm::vec3>& positions);

    int length() const;

    void add(const glm::vec3& position);
    void join(const Path& other);
    void removeFirst(int count = 1);

    const glm::vec3& operator[](int index) const;
};

struct CarPath : public Path {
    std::queue<glm::ivec2> roadNodes;

    /// @brief Returns the current segment of the path
    /// @return Start, end and connection vector (end - start) of the first path segment
    std::tuple<glm::vec3, glm::vec3, glm::vec3> getCurrentSegment() const;

    std::tuple<glm::ivec2, glm::ivec2> getPathEndTile() const;
};