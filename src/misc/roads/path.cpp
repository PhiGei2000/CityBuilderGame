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
#include "misc/roads/path.hpp"
#include "misc/coordinateTransform.hpp"

Path::Path() {
}

Path::Path(const std::initializer_list<glm::vec3>& positions)
    : positions(positions) {
}

int Path::length() const {
    return positions.size();
}

void Path::add(const glm::vec3& position) {
    positions.push_back(position);
}

void Path::join(const Path& other) {
    positions.insert(positions.end(), other.positions.begin(), other.positions.end());
}

void Path::removeFirst(int count) {
    if (count >= length()) {
        positions.clear();
    }

    positions.erase(positions.begin(), positions.begin() + count);
}

const glm::vec3& Path::operator[](int index) const {
    return positions[index];
}

std::tuple<glm::vec3, glm::vec3, glm::vec3> CarPath::getCurrentSegment() const {
    const glm::vec3& start = positions[0];
    const glm::vec3& end = positions[1];
    const glm::vec3& pathVector = end - start;

    return std::tuple(start, end, pathVector);
}

std::tuple<glm::ivec2, glm::ivec2> CarPath::getPathEndTile() const {
    auto it = positions.rbegin();

    const glm::vec3& lastSegmentEnd = *(it++);
    const glm::vec3& lastSegmentStart = *it;
    const glm::vec3& lastSegmentDir = glm::normalize(lastSegmentEnd - lastSegmentStart);

    return utility::worldToNormalizedChunkGridCoords(lastSegmentEnd + lastSegmentDir);
}