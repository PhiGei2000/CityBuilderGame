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
#include "../constraints.hpp"
#include "../rectangle.hpp"

#include "../eventDispatcher.hpp"

#include <string>

#include <glm/glm.hpp>

class Gui;
struct MouseMoveEvent;
struct MouseButtonEvent;

class Widget {
  protected:
    Gui* gui;
    bool visible = false;

    friend class Gui;

  public:
    const std::string id;

    EventDispatcher<MouseMoveEvent> onMouseEnter;
    EventDispatcher<MouseMoveEvent> onMouseLeave;

    Widget(const std::string& id, Gui* gui, const glm::vec4& backgroundColor);

    Widget* parent = nullptr;
    glm::vec4 backgroundColor;
    float cornerRadius = 0.0f;
    Constraints constraints;

    virtual void show();
    virtual void hide();
    bool isVisible() const;

    virtual void update();

    virtual void render() const;

    virtual Rectangle getBox() const;

    virtual void handleMouseButtonEvent(MouseButtonEvent& e);
    virtual void handleMouseMoveEvent(MouseMoveEvent& e);
};
