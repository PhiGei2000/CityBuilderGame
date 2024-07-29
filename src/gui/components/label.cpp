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

Label::Label(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, const int textSize, const glm::vec4& textColor)
    : Widget(id, gui, backgroundColor), text(text), textSize(textSize), textColor(textColor) {
}

Rectangle Label::getBox() const {
    if (constraints.height.type != ConstraintType::FIT_TO_CONTENT && constraints.width.type != ConstraintType::FIT_TO_CONTENT) {
        return Widget::getBox();
    }

    Rectangle parentBox = Widget::getBox();

    if (constraints.height.type == ConstraintType::FIT_TO_CONTENT) {
        float baselineoffset;
        float height = gui->textRenderer.getHeight(text, textSize, &baselineoffset);
        parentBox.height = height;
    }

    if (constraints.width.type == ConstraintType::FIT_TO_CONTENT) {
        float width = gui->textRenderer.getWidth(text, textSize);
        parentBox.width = width;
    }

    return parentBox;
}

void Label::render() const {
    if (!visible) {
        return;
    }

    Widget::render();

    const Rectangle& box = getBox();
    ShaderProgram* guiShader = gui->getShader();

    guiShader->setBool("flipV", true);
    guiShader->setBool("text", true);
    guiShader->setVector4("color", textColor);

    gui->textRenderer.renderText(text, box, textSize, textAlign);

    guiShader->setBool("flipV", false);
    guiShader->setBool("text", false);
}
