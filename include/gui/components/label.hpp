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
#include "gui/colors.hpp"
#include "rendering/geometry.hpp"
#include "resources/font.hpp"
#include "widget.hpp"

#include <string>

enum class TextAlign {
    BEGIN,
    CENTER,
    END
};

class Label : public virtual Widget {
  protected:
    Geometry geometry;
    float textWidth, textHeight, baselineOffset = 0;
    // TextRenderer textRenderer;

    struct TextVertex {
        glm::vec2 position;
        glm::vec2 texCoord;

        static const VertexAttributes attributes;
    };

  public:
    std::string text;
    TextAlign textAlign;
    glm::vec4 textColor;
    FontPtr font;
    int textSize;

    Label(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, FontPtr font = nullptr, const int textSize = 24, TextAlign align = TextAlign::BEGIN, const glm::vec4& textColor = colors::white);

    void updateTextGeometry();

    virtual void update() override;

    virtual Rectangle getBox() const override;

    virtual void render() const override;
};
