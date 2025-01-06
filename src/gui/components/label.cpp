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
#include "gui/components/label.hpp"
#include "gui/gui.hpp"
#include "resources/font.hpp"

#include "application.hpp"

const VertexAttributes Label::TextVertex::attributes = {
    VertexAttribute{3, GL_FLOAT, GL_FALSE, 4 * sizeof(float),                 0},
    VertexAttribute{2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 2 * sizeof(float)}
};

Label::Label(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, FontPtr font, const int textSize, TextAlign textAlign, const glm::vec4& textColor)
    : Widget(id, gui, backgroundColor), text(text), textAlign(textAlign), textColor(textColor), textSize(textSize), geometry(TextVertex::attributes) {
    this->font = font == nullptr ? gui->getApp()->getGame()->getResourceManager().getResource<Font>("Montserrat-Regular") : font;
}

Rectangle Label::getBox() const {
    if (constraints.height.type != ConstraintType::FIT_TO_CONTENT && constraints.width.type != ConstraintType::FIT_TO_CONTENT) {
        return Widget::getBox();
    }

    Rectangle parentBox = Widget::getBox();

    if (constraints.height.type == ConstraintType::FIT_TO_CONTENT) {
        parentBox.height = textHeight;
    }

    if (constraints.width.type == ConstraintType::FIT_TO_CONTENT) {
        parentBox.width = textWidth;
    }

    return parentBox;
}

void Label::updateTextGeometry() {
    std::vector<TextVertex> vertices = std::vector<TextVertex>();
    vertices.reserve(4 * text.size());
    std::vector<unsigned int> indices = std::vector<unsigned int>();
    indices.reserve(6 * text.size());

    float screenWidth, screenHeight;
    gui->getScreenSize(&screenWidth, &screenHeight);

    float pixelScale = 1.0f / 24.0f;// / screenHeight;

    glm::vec2 currentPos = {0.0f, 0.0f};
    unsigned int indexOffset = 0;
    unsigned int order[] = {0, 1, 2, 2, 1, 3};

    textHeight = 0;
    for (const char ch : text) {
        glm::vec2 lastPos = currentPos;
        GlyphInfo info = font->getGlyphInfo(ch, &currentPos);

        float characterWidth = info.positions[1].x - info.positions[0].x;
        currentPos = info.offset - glm::vec2((1 - textSize * pixelScale) * characterWidth, 0.0f);

        vertices.emplace_back(pixelScale * textSize * info.positions[0], info.texCoords[0]);
        vertices.emplace_back(pixelScale * textSize * info.positions[1], info.texCoords[1]);
        vertices.emplace_back(pixelScale * textSize * info.positions[2], info.texCoords[2]);
        vertices.emplace_back(pixelScale * textSize * info.positions[3], info.texCoords[3]);

        for (int i = 0; i < 6; i++) {
            indices.push_back(indexOffset + order[i]);
        }

        indexOffset += 4;

        textHeight = std::max(textHeight, pixelScale * textSize * (info.positions[0].y - info.positions[1].y));
        baselineOffset = std::min(baselineOffset, -pixelScale * textSize * info.positions[0].y);
    }

    textWidth = currentPos.x;

    Rectangle box = getBox();
    glm::vec2 textPos = {box.x, box.y + box.height * 0.75f};
    switch (textAlign) {
        case TextAlign::BEGIN:
            textPos.x += 10.0f;
            break;
        case TextAlign::CENTER:
            textPos.x += 0.5 * (box.width - textWidth);
            break;
        case TextAlign::END:
            textPos.x += box.width - textWidth - 10.0f;
            break;
    }

    for (int i = 0; i < vertices.size(); i++) {
        vertices[i].position += textPos;
    }

    geometry.bufferData(vertices, indices, GL_DYNAMIC_DRAW);
}

void Label::update() {
    updateTextGeometry();
}

void Label::render() const {
    if (!visible) {
        return;
    }

    Widget::render();

    ShaderProgram* guiShader = gui->getShader();

    guiShader->setBool("text", true);
    guiShader->setVector4("color", textColor);

    font->use();
    geometry.draw();

    guiShader->setBool("text", false);
}
