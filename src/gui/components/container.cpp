#include "gui/components/container.hpp"

ContainerItem::ContainerItem(Gui* gui, GuiElement* element, const glm::vec4 backgroundColor)
    : GuiElement(gui, backgroundColor), element(element) {
    element->parent = this;
}

void ContainerItem::render() const {
    GuiElement::render();

    element->render();
}

Container::Container(Gui* gui, const glm::vec4& backgroundColor)
    : GuiElement(gui, backgroundColor) {
}

void Container::addChild(GuiElement* child) {
    ContainerItem* item = new ContainerItem(gui, child);
    item->parent = this;

    children.push_back(item);

    setChildConstraints();
}

void Container::render() const {
    GuiElement::render();

    for (const GuiElement* child : children) {
        child->render();
    }
}