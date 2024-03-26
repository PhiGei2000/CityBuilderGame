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
#include "gui/components/stackPanel.hpp"

StackPanel::StackPanel(const std::string& id, Gui* gui, StackOrientation orientation, const glm::vec4 backgroundColor, ItemAligment itemAligment)
    : Container(id, gui, backgroundColor), orientation{orientation}, itemAligment{itemAligment} {
}

void StackPanel::setChildConstraints() {
    for (const auto& child : children) {
        Container* container = dynamic_cast<Container*>(child);
        if (container != nullptr) {
            container->setChildConstraints();
        }
    }

    Rectangle containerBox = getBox();

    if (!outerSpacing) {
        if (orientation == StackOrientation::COLUMN || orientation == StackOrientation::COLUMN_REVERSE) {
            containerBox.height += spacing;
        }
        else {
            containerBox.width += spacing;
        }
    }

    switch (itemAligment) {
        case ItemAligment::STRECH: {
            float spacing = 1.0f / children.size();

            switch (orientation) {
                case StackOrientation::COLUMN:
                    for (int i = 0; i < children.size(); i++) {
                        children[i]->constraints.y = RelativeConstraint(i * spacing);
                        children[i]->constraints.height = RelativeConstraint(spacing);
                    }
                    break;
                case StackOrientation::COLUMN_REVERSE:
                    for (int i = 0; i < children.size(); i++) {
                        children[i]->constraints.y = RelativeConstraint(1 - (i + 1) * spacing);
                        children[i]->constraints.height = RelativeConstraint(spacing);
                    }
                    break;
                case StackOrientation::ROW:
                    for (int i = 0; i < children.size(); i++) {
                        children[i]->constraints.x = RelativeConstraint(i * spacing);
                        children[i]->constraints.width = RelativeConstraint(spacing);
                    }
                    break;
                case StackOrientation::ROW_REVERSE:
                    for (int i = 0; i < children.size(); i++) {
                        children[i]->constraints.x = RelativeConstraint(1 - (i + 1) * spacing);
                        children[i]->constraints.width = RelativeConstraint(spacing);
                    }
                default:
                    break;
            }
        } break;
        case ItemAligment::BEGIN: {
            float current = spacing / 2.0f;

            switch (orientation) {
                case StackOrientation::COLUMN:
                    for (int i = 0; i < children.size(); i++) {
                        children[i]->constraints.y = AbsoluteConstraint(current);

                        const Rectangle& childArea = children[i]->getBox();
                        current += (childArea.height + spacing);
                    }
                    break;
                case StackOrientation::COLUMN_REVERSE:
                    for (int i = children.size() - 1; i >= 0; i--) {
                        children[i]->constraints.y = AbsoluteConstraint(current);

                        const Rectangle& childArea = children[i]->getBox();
                        current += (childArea.height + spacing);
                    }
                    break;
                case StackOrientation::ROW:
                    for (int i = 0; i < children.size(); i++) {
                        children[i]->constraints.x = AbsoluteConstraint(current);

                        const Rectangle& childArea = children[i]->getBox();
                        current += (childArea.width + spacing);
                    }
                    break;
                case StackOrientation::ROW_REVERSE:
                    for (int i = children.size() - 1; i >= 0; i--) {
                        children[i]->constraints.x = AbsoluteConstraint(current);

                        const Rectangle& childArea = children[i]->getBox();
                        current += (childArea.width + spacing);
                    }
                default:
                    break;
            }
        } break;
        case ItemAligment::CENTER: {
            float current = 0;
            switch (orientation) {
                case StackOrientation::COLUMN: {
                    float height = 0.0f;
                    for (const auto child : children) {
                        const Rectangle& childArea = child->getBox();
                        height += (childArea.height + spacing);
                    }

                    current = (containerBox.height - height + spacing) / 2.0f;
                    for (int i = 0; i < children.size(); i++) {
                        children[i]->constraints.y = AbsoluteConstraint(current);

                        const Rectangle& childArea = children[i]->getBox();
                        current += (childArea.height + spacing);
                    }
                } break;
                case StackOrientation::COLUMN_REVERSE: {
                    float height = 0.0f;
                    for (const auto child : children) {
                        const Rectangle& childArea = child->getBox();
                        height += (childArea.height + spacing);
                    }

                    current = (containerBox.height - height + spacing) / 2.0f;
                    for (int i = children.size() - 1; i >= 0; i--) {
                        children[i]->constraints.y = AbsoluteConstraint(current);

                        const Rectangle& childArea = children[i]->getBox();
                        current += (childArea.height + spacing);
                    }
                } break;
                case StackOrientation::ROW: {
                    float width = 0.0f;
                    for (const auto child : children) {
                        const Rectangle& childArea = child->getBox();
                        width += (childArea.width + spacing);
                    }

                    current = (containerBox.width - width + spacing) / 2.0f;
                    for (int i = 0; i < children.size(); i++) {
                        children[i]->constraints.x = AbsoluteConstraint(current);

                        const Rectangle& childArea = children[i]->getBox();
                        current += (childArea.width + spacing);
                    }
                } break;
                case StackOrientation::ROW_REVERSE: {
                    float width = 0.0f;
                    for (const auto child : children) {
                        const Rectangle& childArea = child->getBox();
                        width += (childArea.width + spacing);
                    }

                    current = (containerBox.width - width + spacing) / 2.0f;
                    for (int i = children.size() - 1; i >= 0; i--) {
                        children[i]->constraints.x = AbsoluteConstraint(current);

                        const Rectangle& childArea = children[i]->getBox();
                        current += (childArea.width + spacing);
                    }
                } break;
                default:
                    break;
            }
        } break;
        case ItemAligment::END: {
            float current = 0.0f;

            switch (orientation) {
                case StackOrientation::COLUMN: {
                    float height = 0.0f;
                    for (const Widget* child : children) {
                        const Rectangle& area = child->getBox();
                        height += (area.height + spacing);
                    }

                    current = (containerBox.height - height) + spacing / 2.0f;

                    for (int i = 0; i < children.size(); i++) {
                        children[i]->constraints.y = AbsoluteConstraint(current);

                        const Rectangle& childArea = children[i]->getBox();
                        current += (childArea.height + spacing);
                    }
                } break;
                case StackOrientation::COLUMN_REVERSE: {
                    float height = 0.0f;
                    for (const Widget* child : children) {
                        const Rectangle& area = child->getBox();
                        height += (area.height + spacing);
                    }

                    current = (containerBox.height - height) + spacing / 2.0f;

                    for (int i = children.size() - 1; i >= 0; i--) {
                        children[i]->constraints.y = AbsoluteConstraint(current);

                        const Rectangle& childArea = children[i]->getBox();
                        current += (childArea.height + spacing);
                    }
                } break;
                case StackOrientation::ROW: {
                    float width = 0.0f;
                    for (const Widget* child : children) {
                        const Rectangle& area = child->getBox();
                        width += (area.width + spacing);
                    }

                    current = (containerBox.width - width) + spacing / 2.0f;

                    for (int i = 0; i < children.size(); i++) {
                        children[i]->constraints.x = AbsoluteConstraint(current);

                        const Rectangle& childArea = children[i]->getBox();
                        current += (childArea.width + spacing);
                    }
                } break;
                case StackOrientation::ROW_REVERSE: {
                    float width = 0.0f;
                    for (const Widget* child : children) {
                        const Rectangle& area = child->getBox();
                        width += (area.width + spacing);
                    }

                    current = (containerBox.width - width) + spacing / 2.0f;

                    for (int i = children.size() - 1; i >= 0; i--) {
                        children[i]->constraints.x = AbsoluteConstraint(current);

                        const Rectangle& childArea = children[i]->getBox();
                        current += (childArea.width + spacing);
                    }
                } break;
                default:
                    break;
            }
        } break;
    }
}

Rectangle StackPanel::getBox() const {
    Rectangle box = Widget::getBox();

    if (!outerSpacing) {
        if (orientation == StackOrientation::COLUMN || orientation == StackOrientation::COLUMN_REVERSE) {
            box.height -= spacing;
        }
        else {
            box.width -= spacing;
        }
    }

    return box;
}
