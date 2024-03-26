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
#include "gui/rectangle.hpp"
#include "renderQuad.hpp"
#include "shader.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>

#include <map>

enum class TextAlign {
    BEGIN,
    CENTER,
    END
};

class TextRenderer {
  private:
    struct Character {
        unsigned int textureId;
        glm::ivec2 size;
        glm::ivec2 bearing;
        unsigned int advance;
    };

    RenderQuad quad;
    float screenWidth, screenHeight;

    std::map<char, Character> characters;

  public:
    glm::vec3 textColor = colors::white;

    void init();
    void setScreenSize(float width, float height);

    float getWidth(const std::string& text, float scale) const;
    float getHeight(const std::string& text, float scale, float* baseLineOffset) const;

    void renderText(const std::string& text, const Rectangle& rect, float maxScale, TextAlign align) const;
};
