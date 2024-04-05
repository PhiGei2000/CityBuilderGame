// /*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
//  *
//  *  This program is free software: you can redistribute it and/or modify
//  *  it under the terms of the GNU General Public License as published by
//  *  the Free Software Foundation, either version 3 of the License, or
//  *  (at your option) any later version.
//  *
//  *  This program is distributed in the hope that it will be useful,
//  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  *  GNU General Public License for more details.
//  *
//  *  You should have received a copy of the GNU General Public License
//  *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//  */
// #include "resources/mesh.hpp"

// #include "misc/configuration.hpp"
// #include "rendering/instanceBuffer.hpp"
// #include "rendering/shadowBuffer.hpp"

// #include "misc/roads/roadTile.hpp"

// template<>
// constexpr VertexAttributes getInstanceBufferVertexAttributes<TransformationComponent>() {
//     return VertexAttributes{
//         VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(0 * sizeof(glm::vec4)), 0, 1},
//         VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)), 0, 1},
//         VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)), 0, 1},
//         VertexAttribute{4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)), 0, 1},
//     };
// }

// template<>
// constexpr VertexAttributes getInstanceBufferVertexAttributes<RoadRenderData>() {
//     int stride = sizeof(glm::vec3) + sizeof(int);

//     return VertexAttributes{
//         VertexAttribute{3, GL_FLOAT, GL_FALSE, stride,                   (void*)0, 0, 1},
//         VertexAttribute{1,   GL_INT, GL_FALSE, stride, (void*)(sizeof(glm::vec3)), 0, 1},
//     };
// }
