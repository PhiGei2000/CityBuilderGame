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
#include "rendering/vertex.hpp"

#include <array>
#include <functional>
#include <unordered_set>
#include <vector>

#if DEBUG
#include <string>
#endif

struct GeometryData {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    bool culling;

    GeometryData();

    GeometryData(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, bool culling = true);

    static GeometryData merge(const GeometryData& first, const GeometryData& second);

    void addData(const GeometryData& data);

    GeometryData transformVertices(const std::function<Vertex(const Vertex&)>& transformation) const;
    GeometryData transformVertices(const glm::mat4& transform) const;

    static std::pair<glm::vec3, glm::vec3> calculateTangentSpace(const std::array<Vertex, 3>& triangle);
    static void calculateTangentSpace(Vertex& v1, Vertex& v2, Vertex& v3);

    void calculateTangentSpace();

#if DEBUG
    void save(const std::string& filename) const;
#endif
};
