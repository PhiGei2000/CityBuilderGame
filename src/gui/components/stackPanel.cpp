#include "gui/components/stackPanel.hpp"

StackPanel::StackPanel(const std::string& id, Gui* gui, StackOrientation orientation, const glm::vec4 backgroundColor)
    : Container(id, gui, backgroundColor), orientation{orientation} {
}

void StackPanel::setChildConstraints() {
    const Rectangle& containerBox = getBox();

    float spacing = 1.0f / children.size();
    switch (orientation) {
    case StackOrientation::COLUMN:
        for (int i = 0; i < children.size(); i++) {
            children[i]->constraints.y = RelativeConstraint(1 - (i + 1) * spacing);
            children[i]->constraints.height = RelativeConstraint(spacing);
        }
        break;
    case StackOrientation::COLUMN_REVERSE:
        for (int i = 0; i < children.size(); i++) {
            children[i]->constraints.y = RelativeConstraint(i * spacing);
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
}
