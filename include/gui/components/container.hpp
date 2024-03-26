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
#include "../colors.hpp"
#include "widget.hpp"

#include <vector>

class Container : public Widget {
  protected:
    std::vector<Widget*> children;

  public:
    Container(const std::string& id, Gui* gui, const glm::vec4& backgroundColor);
    virtual void addChild(Widget* child);
    Widget* getChild(const std::string& id) const;

    virtual void show() override;
    virtual void hide() override;

    void update() override;

    void render() const override;

    virtual void setChildConstraints() = 0;

    void handleMouseButtonEvent(MouseButtonEvent& e) override;
    void handleMouseMoveEvent(MouseMoveEvent& e) override;
};
