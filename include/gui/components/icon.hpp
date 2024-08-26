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
#include "widget.hpp"
#include "gui/colors.hpp"

#include "rendering/texture.hpp"

class Icon : public virtual Widget {
  protected:
    Texture* texture;

  public:
    Icon(const std::string& id, Gui* gui, Texture* texture, const glm::vec4& backgroundColor = colors::transparent);
    ~Icon();

    void render() const override;
};
