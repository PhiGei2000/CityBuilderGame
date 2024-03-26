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
#include <glm/glm.hpp>

namespace colors {
    static constexpr glm::vec4 anthraziteGrey = glm::vec4(0.29f, 0.314f, 0.329f, 0.7f);
    static constexpr glm::vec4 white = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    static constexpr glm::vec4 black = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    static constexpr glm::vec4 transparent = glm::vec4(0.0f);

    static constexpr glm::vec4 warning = glm::vec4(0.9f, 0.1, 0.1f, 1.0f);
} // namespace colors
