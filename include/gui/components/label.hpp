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
#include "../colors.hpp"
#include "widget.hpp"
#include "../colors.hpp"

#include "rendering/textRenderer.hpp"
#include <string>

class Label : public virtual Widget {
  public:
    std::string text;
    glm::vec4 textColor;
    int textSize;

    TextAlign textAlign = TextAlign::CENTER;

    Label(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, const int textSize = 24, const glm::vec4& textColor = colors::white);

    virtual Rectangle getBox() const;

    virtual void render() const override;
};
