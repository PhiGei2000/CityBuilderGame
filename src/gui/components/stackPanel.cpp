#include "gui/components/stackPanel.hpp"

#include <set>

StackPanel::StackPanel(const std::string& id, Gui* gui, StackOrientation orientation, const glm::vec4 backgroundColor, ItemAligment itemAligment)
    : Container(id, gui, backgroundColor), orientation{orientation}, itemAligment{itemAligment} {
}

float StackPanel::calculateSpace(float totalSpace, float totalSpacing, const std::function<float(Widget*)>& getChildSize, const std::function<Constraint&(Widget*)>& getChildSizeConstraint) {
    std::set<int> flexItems;
    float remainingSpace = 0.0f;
    float flexSum = 0.0f;
    float space = 0.0f;

    for (int i = 0; i < children.size(); i++) {
        if (children[i]->constraints.width.getType() == ConstraintType::FLEX) {
            flexItems.insert(i);
            flexSum += getChildSizeConstraint(children[i]).getValue<FlexConstraint>().value;
        }
        else {
            space += getChildSize(children[i]);
        }
    }
    remainingSpace = totalSpace - space - totalSpacing;
    for (int i : flexItems) {
        FlexConstraint& flex = getChildSizeConstraint(children[i]).getValue<FlexConstraint>();
        flex.absoluteValue = remainingSpace * flex.value / flexSum;
        space += flex.absoluteValue;
    }

    return space;
}

void StackPanel::setChildPositions(float start, const std::function<float(Widget*, float)>& setChildPosition, bool reverse) {
    float current = start;
    if (reverse) {
        for (int i = children.size() - 1; i >= 0; i--) {
            current += setChildPosition(children[i], current) + spacing;
        }
    }
    else {
        for (int i = 0; i < children.size(); i++) {
            current += setChildPosition(children[i], current) + spacing;
        }
    }
}

void StackPanel::setChildConstraints() {
    Container::setChildConstraints();

    Rectangle containerBox = getBox();

    float totalSpacing = (children.size() - 1) * spacing;
    std::function<float(Widget*, float)> setChildPosition;
    bool reverse = false;
    float totalSpace = 0.0f;

    float space = 0.0f;
    switch (orientation) {
        case StackOrientation::COLUMN_REVERSE:
            reverse = true;
        case StackOrientation::COLUMN:
            totalSpace = containerBox.height;
            space = calculateSpace(totalSpace, totalSpacing, [](Widget* child) { return child->getBox().height; }, [](Widget* child) -> Constraint& { return child->constraints.height; });

            setChildPosition = [](Widget* child, float position) {
                child->constraints.y = AbsoluteConstraint(position);
                child->invalidate();

                return child->getBox().height;
            };
            break;
        case StackOrientation::ROW_REVERSE:
            reverse = true;
        case StackOrientation::ROW:
            totalSpace = containerBox.width;
            space = calculateSpace(totalSpace, totalSpacing, [](Widget* child) { return child->getBox().width; }, [](Widget* child) -> Constraint& { return child->constraints.width; });

            setChildPosition = [](Widget* child, float position) {
                child->constraints.x = AbsoluteConstraint(position);
                child->invalidate();

                return child->getBox().width;
            };
            break;
    }

    float start = 0.0f;

    if (itemAligment == ItemAligment::CENTER) {
        start = (totalSpace - space - totalSpacing) / 2.0f;
    }

    setChildPositions(start, setChildPosition, reverse);
}

void StackPanel::applyConstraints() {
    Container::applyConstraints();

    if (constraints.height.getType() == ConstraintType::FIT_TO_CONTENT) {
        float maxHeight = 0.0f;
        for (const auto child : children) {
            const Rectangle& childBox = child->getBox();
            if (childBox.height > maxHeight) {
                maxHeight = childBox.height;
            }
        }

        box.height = maxHeight;
    }

    if (constraints.width.getType() == ConstraintType::FIT_TO_CONTENT) {
        float maxWidth = 0.0f;
        for (const auto child : children) {
            const Rectangle& childBox = child->getBox();
            if (childBox.width > maxWidth) {
                maxWidth = childBox.width;
            }
        }

        box.width = maxWidth;
    }
}
