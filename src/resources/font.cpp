#include "resources/font.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

Font::Font(const std::string& filename) {
    charInfo = new stbtt_packedchar[charCount];

    loadGlyphs(filename);
}

Font::~Font() {
    delete[] charInfo;
}

void Font::loadGlyphs(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    const auto fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> fileData = std::vector<unsigned char>(fileSize);
    file.read(reinterpret_cast<char*>(&fileData[0]), fileSize);
    file.close();

    auto atlasData = new unsigned char[atlasWidth * atlasHeight];

    stbtt_pack_context context;
    if (!stbtt_PackBegin(&context, atlasData, atlasWidth, atlasHeight, 0, 1, nullptr)) {
        std::cerr << "Failed to initialize font: " << filename << std::endl;
    }

    stbtt_PackSetOversampling(&context, oversampleX, oversampleY);
    if (!stbtt_PackFontRange(&context, fileData.data(), 0, size, firstChar, charCount, charInfo)) {
        std::cerr << "Failed to pack font: " << filename << std::endl;
    }

    stbtt_PackEnd(&context);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, atlasData);
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);

    delete[] atlasData;
}

GlyphInfo Font::getGlyphInfo(char character, glm::vec2* pos) const {
    stbtt_aligned_quad quad;

    GlyphInfo info;
    stbtt_GetPackedQuad(charInfo, atlasWidth, atlasHeight, character - firstChar, &pos->x, &pos->y, &quad, 1);
    info.offset = *pos;

    info.positions[0] = glm::vec2(quad.x0, quad.y1);
    info.positions[1] = glm::vec2(quad.x0, quad.y0);
    info.positions[2] = glm::vec2(quad.x1, quad.y1);
    info.positions[3] = glm::vec2(quad.x1, quad.y0);

    info.texCoords[0] = glm::vec2(quad.s0, quad.t1);
    info.texCoords[1] = glm::vec2(quad.s0, quad.t0);
    info.texCoords[2] = glm::vec2(quad.s1, quad.t1);
    info.texCoords[3] = glm::vec2(quad.s1, quad.t0);

    info.xAdvance = charInfo[character - firstChar].xadvance;

    return info;
}

void Font::use() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}