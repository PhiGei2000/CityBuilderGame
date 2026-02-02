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

void StackPanel::setChildPositions(const glm::vec2& offset, const std::function<glm::vec2(Widget*, const glm::vec2&)>& setChildPosition, bool reverse) {
    glm::vec2 current = offset;
    if (reverse) {
        for (int i = children.size() - 1; i >= 0; i--) {
            current += setChildPosition(children[i], current);
        }
    }
    else {
        for (int i = 0; i < children.size(); i++) {
            current += setChildPosition(children[i], current);
        }
    }
}

void StackPanel::setChildConstraints() {
    Container::setChildConstraints();
    if (children.size() == 0) {
        return;
    }

    const Rectangle& containerBox = getBox();
    const Rectangle& childArea = getChildArea();
    glm::vec2 offset = childArea.getPosition() - containerBox.getPosition();

    float totalSpacing = (children.size() - 1) * spacing;
    std::function<glm::vec2(Widget*, const glm::vec2&)> setChildPosition;
    bool reverse = false;
    float totalSpace = 0.0f;

    float space = 0.0f;
    switch (orientation) {
        case StackOrientation::COLUMN_REVERSE:
            reverse = true;
        case StackOrientation::COLUMN:
            totalSpace = childArea.height;
            space = calculateSpace(totalSpace, totalSpacing, [](Widget* child) { return child->getBox().height; }, [](Widget* child) -> Constraint& { return child->constraints.height; });

            setChildPosition = [this](Widget* child, const glm::vec2& position) {
                child->constraints.x = AbsoluteConstraint(position.x);
                child->constraints.y = AbsoluteConstraint(position.y);
                child->invalidate();

                return glm::vec2(0.0f, child->getBox().height + spacing);
            };

            if (itemAligment == ItemAligment::CENTER) {
                offset.y += (totalSpace - space - totalSpacing) / 2.0f;
            }
            break;
        case StackOrientation::ROW_REVERSE:
            reverse = true;
        case StackOrientation::ROW:
            totalSpace = childArea.width;
            space = calculateSpace(totalSpace, totalSpacing, [](Widget* child) { return child->getBox().width; }, [](Widget* child) -> Constraint& { return child->constraints.width; });

            setChildPosition = [this](Widget* child, const glm::vec2& position) {
                child->constraints.x = AbsoluteConstraint(position.x);
                child->constraints.y = AbsoluteConstraint(position.y);
                child->invalidate();

                return glm::vec2(child->getBox().width + spacing, 0.0f);
            };

            if (itemAligment == ItemAligment::CENTER) {
                offset.x += (totalSpace - space - totalSpacing) / 2.0f;
            }
            break;
    }

    setChildPositions(offset, setChildPosition, reverse);
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
