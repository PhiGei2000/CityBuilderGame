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
#include "rendering/textRenderer.hpp"

#include <GL/glew.h>
#include <fstream>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

void TextRenderer::init() {
    // FT_Library ft;
    // if (FT_Init_FreeType(&ft)) {
    //     std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    // }

    // FT_Face face;
    // if (FT_New_Face(ft, filename, 0, &face)) {
    //     std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    // }

    // useKerning = FT_HAS_KERNING(face);

    // FT_Set_Pixel_Sizes(face, 0, pixelWidth);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // for (unsigned char c = 0; c < 128; c++) {
    //     if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
    //         std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
    //         continue;
    //     }

    //     Character character;
    //     glGenTextures(1, &character.textureId);
    //     glBindTexture(GL_TEXTURE_2D, character.textureId);
    //     glTexImage2D(
    //         GL_TEXTURE_2D,
    //         0,
    //         GL_RED,
    //         face->glyph->bitmap.width,
    //         face->glyph->bitmap.rows,
    //         0,
    //         GL_RED,
    //         GL_UNSIGNED_BYTE,
    //         face->glyph->bitmap.buffer);

    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    //     character.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
    //     character.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
    //     character.advance = face->glyph->advance.x;

    //     characters[c] = character;

    //     if (useKerning) {
    //         FT_Vector vec;
    //         for (unsigned char d = 0; d < 128; d++) {
    //             FT_Get_Kerning(face, c, d, FT_KERNING_DEFAULT, &vec);

    //             kerning.emplace(std::make_pair(c, d), glm::ivec2{vec.x, vec.y});
    //         }
    //     }
    // }
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    // glBindTexture(GL_TEXTURE_2D, 0);

    // FT_Done_Face(face);
    // FT_Done_FreeType(ft);

    // read file
    static constexpr const char* filename = "res/fonts/Montserrat-Regular.ttf";
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    const auto size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> bytes = std::vector<uint8_t>(size);
    file.read(reinterpret_cast<char*>(&bytes[0]), size);
    file.close();

    auto atlasData = std::make_unique<uint8_t>(font.atlasWidth * font.atlasHeight);

    stbtt_pack_context context;
    if (!stbtt_PackBegin(&context, atlasData.get(), font.atlasWidth, font.atlasHeight, 0, 1, nullptr)) {
        std::cerr << "Failed to initialize font" << std::endl;
    }

    stbtt_PackSetOversampling(&context, font.oversampleX, font.oversampleY);
    if (!stbtt_PackFontRange(&context, bytes.data(), 0, font.size, font.firstChar, font.charCount, font.charInfo.get())) {
        std::cerr << "Failed to pack font" << std::endl;
    }

    stbtt_PackEnd(&context);

    glGenTextures(1, &font.texture);
    glBindTexture(GL_TEXTURE_2D, font.texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, font.atlasWidth, font.atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, atlasData.get());
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);

    initGlyphs();
}

void TextRenderer::initGlyphs() {
    for (int ch = 0; ch < font.charCount; ch++) {
        stbtt_aligned_quad quad;

        GlyphInfo info;
        stbtt_GetPackedQuad(font.charInfo.get(), font.atlasWidth, font.atlasHeight, ch, &info.offsetX, &info.offsetY, &quad, 1);

        info.positions[0] = glm::vec3(quad.x0, -quad.y1, 0);
        info.positions[1] = glm::vec3(quad.x0, -quad.y0, 0);
        info.positions[2] = glm::vec3(quad.x1, -quad.y1, 0);
        info.positions[3] = glm::vec3(quad.x1, -quad.y0, 0);
        info.texCoords[0] = glm::vec2(quad.s0, quad.t1);
        info.texCoords[1] = glm::vec2(quad.s0, quad.t0);
        info.texCoords[2] = glm::vec2(quad.s1, quad.t1);
        info.texCoords[3] = glm::vec2(quad.s1, quad.t0);
    }
}

void TextRenderer::setScreenSize(float width, float height) {
    screenWidth = width;
    screenHeight = height;
}

float TextRenderer::getWidth(const std::string& text, int textSize) const {
    float width = 0;
    for (auto it = text.begin(); it != text.end(); it++) {
        const Character& character = characters.at(*it);
        width += (character.advance >> 6);
    }

    return width * textSize / pixelWidth;
}

float TextRenderer::getHeight(const std::string& text, int textSize, float* baselineOffset) const {
    *baselineOffset = 0;

    float height = 0.0f;
    for (auto it = text.begin(); it != text.end(); it++) {
        const Character& character = characters.at(*it);
        if (height < character.size.y) {
            height = character.size.y;
        }

        float glyphBaselineOffset = (character.size.y - character.bearing.y);
        if (glyphBaselineOffset > *baselineOffset) {
            *baselineOffset = glyphBaselineOffset;
        }
    }

    *baselineOffset *= textSize / pixelWidth;
    return height * textSize / pixelWidth;
}

void TextRenderer::renderText(const std::string& text, const Rectangle& rect, int textSize, TextAlign align) const {
    // get width and height of non scaled text
    float baselineOffset;

    float width = getWidth(text, textSize);
    float height = getHeight(text, textSize, &baselineOffset);

    float currentX;
    switch (align) {
        case TextAlign::BEGIN:
            currentX = rect.x;
            break;
        case TextAlign::CENTER:
            currentX = rect.x + (rect.width - width) * 0.5f;
            break;
        case TextAlign::END:
            currentX = rect.x + (rect.width - width);
            break;
        default:
            break;
    }
    float currentY = rect.y + rect.height - (baselineOffset + (rect.height - height) * 0.5f);

    glActiveTexture(GL_TEXTURE0);

    for (auto it = text.begin(); it != text.end(); it++) {
        glm::ivec2 offset(0.0f);

        Character character = characters.at(*it);
        if (it + 1 != text.end() && useKerning) {
            Character nextChar = characters.at(*(it + 1));
            offset = kerning.at(std::make_pair(*it, *(it + 1)));
        }

        float xPos = currentX + (character.bearing.x + (offset.x >> 6)) * textSize / pixelWidth;
        float yPos = currentY - character.bearing.y * textSize / pixelWidth;

        float charHeight = character.size.y * textSize / pixelWidth;
        float charWidth = character.size.x * textSize / pixelWidth;

        glBindTexture(GL_TEXTURE_2D, character.textureId);

        quad.draw(xPos, yPos, charWidth, charHeight);
        currentX += (character.advance >> 6) * textSize / pixelWidth;
    }
}
