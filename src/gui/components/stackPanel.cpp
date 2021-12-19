#include "gui/components/stackPanel.hpp"

StackPanel::StackPanel(Gui* gui, StackOrientation orientation, const glm::vec4 backgroundColor)
    : Container(gui, backgroundColor), orientation{orientation} {
}

void StackPanel::setChildConstraints() {
    const Rectangle& containerBox = getBox();

    float spacing;
    switch (orientation) {
    case StackOrientation::COLUMN:
        spacing = 1.0f / children.size();

        for (int i = 0; i < children.size(); i++) {
            children[i]->constraints.y = RelativeConstraint(1 - (2 * i + 1) * spacing);
        }
        break;
    // case StackOrientation::COLUMN_REVERSE:
    //     spacing = containerBox.height / children.size();
    //     y = -(containerBox.height - spacing) / 2;
    //     for (auto child : children) {
    //         child->constraints.y = AbsoluteConstraint(y);
    //         y += spacing;
    //     }
    //     break;
    // case StackOrientation::ROW:
    //     spacing = containerBox.height / children.size();
    //     x = (containerBox.width - spacing) / 2;
    //     for (auto child : children) {
    //         child->constraints.x = AbsoluteConstraint(x);
    //         x -= spacing;
    //     }
    //     break;
    // case StackOrientation::ROW_REVERSE:
    //     spacing = containerBox.height / children.size();
    //     x = -(containerBox.width - spacing) / 2;
    //     for (auto child : children) {
    //         child->constraints.x = AbsoluteConstraint(x);
    //         x += spacing;
    //     }
    //     break;
    default:
        break;
    }
}
