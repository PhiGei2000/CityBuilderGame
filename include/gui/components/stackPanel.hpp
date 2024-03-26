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
#include "container.hpp"

class StackPanel : public Container {
  public:
    enum class StackOrientation {
        ROW,
        COLUMN,
        ROW_REVERSE,
        COLUMN_REVERSE
    };

    enum class ItemAligment {
        BEGIN,
        END,
        CENTER,
        STRECH
    };

    StackOrientation orientation;
    ItemAligment itemAligment;
    float spacing = 30.0f;
    bool outerSpacing = true;

    StackPanel(const std::string& id, Gui* gui, StackOrientation orientation, const glm::vec4 backgroundColor, ItemAligment itemAligment = ItemAligment::CENTER);

    void setChildConstraints() override;

    Rectangle getBox() const override;
};
