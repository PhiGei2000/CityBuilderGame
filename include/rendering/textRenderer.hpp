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

// #include <ft2build.h>
// #include FT_FREETYPE_H
#include <glm/glm.hpp>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <map>

enum class TextAlign {
    BEGIN,
    CENTER,
    END
};

class TextRenderer {
  private:
    struct GlyphInfo {
        glm::vec3 positions[4];
        glm::vec2 texCoords[4];
        float offsetX = 0;
        float offsetY = 0;
    };

    RenderQuad quad;
    float screenWidth, screenHeight;
    int pixelWidth = 128;

    struct Font {
        const unsigned int size = 40;
        const unsigned int atlasWidth = 1024;
        const unsigned int atlasHeight = 1024;
        const unsigned int oversampleX = 2;
        const unsigned int oversampleY = 2;
        const unsigned int firstChar = ' ';
        const unsigned int charCount = '~' - ' ';
        std::unique_ptr<stbtt_packedchar[]> charInfo;
        unsigned int texture = 0;
    } font;

    std::map<char, GlyphInfo> glyphInfos;

    void initGlyphs();

  public:
    glm::vec3 textColor = colors::white;

    void init();
    void setScreenSize(float width, float height);

    float getWidth(const std::string& text, int textSize) const;
    float getHeight(const std::string& text, int textSize, float* baseLineOffset) const;

    void renderText(const std::string& text, const Rectangle& rect, int textSize, TextAlign align) const;
};
