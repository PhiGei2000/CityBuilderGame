#include "gui/components/container.hpp"

Container::Container(Gui* gui, const glm::vec4& backgroundColor)
    : GuiElement(gui, backgroundColor) {
}

void Container::addChild(GuiElement* child) {
    child->parent = this;

    children.push_back(child);

    setChildConstraints();
}

void Container::render() const {
    for (const GuiElement* child : children) {
        child->render();
    }
}