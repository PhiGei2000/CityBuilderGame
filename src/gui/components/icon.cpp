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
#include "gui/components/icon.hpp"

#include "gui/gui.hpp"

#include <GL/glew.h>

Icon::Icon(const std::string& id, Gui* gui, Texture* texture, const glm::vec4& backgroundColor)
    : Widget(id, gui, backgroundColor), texture(texture) {
}

void Icon::render() const {
    Widget::render();

    glActiveTexture(GL_TEXTURE0);

    const Rectangle& box = getBox();

    Shader* guiShader = gui->getShader();
    guiShader->setInt("tex", 0);
    guiShader->setBool("useTexture", true);

    texture->use(0);

    gui->getRenderQuad().draw(box.x, box.y, box.width, box.height);

    guiShader->setBool("useTexture", false);
}
