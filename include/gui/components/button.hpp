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

#include "icon.hpp"
#include "label.hpp"
#include "widget.hpp"

class Button : public virtual Widget {
  public:
    EventDispatcher<MouseButtonEvent> onClick;

    Button(const std::string& id, Gui* gui, const glm::vec4& backgroundColor);

    void handleMouseButtonEvent(MouseButtonEvent& e) override;
};

class TextButton : public Label, public Button {
  public:
    TextButton(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, int textSize = 24, const glm::vec4& textColor = colors::white);
};

class IconButton : public Icon, public Button {
  public:
    IconButton(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, Texture* icon);
};
