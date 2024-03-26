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

Label::Label(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, const glm::vec4& textColor)
    : Widget(id, gui, backgroundColor), text(text), textColor(textColor) {
}

void Label::render() const {
    if (!visible) {
        return;
    }
    
    Widget::render();

    const Rectangle& box = getBox();
    Shader* guiShader = gui->getShader();

    guiShader->setBool("flipV", true);
    guiShader->setBool("text", true);
    guiShader->setVector4("color", textColor);

    gui->textRenderer.renderText(text, box, 0.2f, textAlign);

    guiShader->setBool("flipV", false);
    guiShader->setBool("text", false);
}
