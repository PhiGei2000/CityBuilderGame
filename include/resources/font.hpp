#pragma once
#include "misc/typedefs.hpp"

#include <map>
#include <string>

#include <stb_truetype.h>

#include <glm/glm.hpp>

struct GlyphInfo {
    glm::vec2 positions[4];
    glm::vec2 texCoords[4];
    glm::vec2 offset = glm::vec2(0.0f);
    float xAdvance;
};

struct Font {
    const unsigned int size = 40;
    const unsigned int atlasWidth = 1024;
    const unsigned int atlasHeight = 1024;
    const unsigned int oversampleX = 2;
    const unsigned int oversampleY = 2;
    const unsigned int firstChar = ' ';
    const unsigned int charCount = '~' - ' ';
    stbtt_packedchar* charInfo;
    unsigned int texture;

    Font(const std::string& filename);
    ~Font();

    GlyphInfo getGlyphInfo(char character, glm::vec2* pos) const;

    void use();

  protected:
    void loadGlyphs(const std::string& filename);

};

using FontPtr = ResourcePtr<Font>;
