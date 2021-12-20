#include "gui/components/container.hpp"

#include <sstream>

ContainerItem::ContainerItem(const std::string& id, Gui* gui, GuiElement* element, const glm::vec4 backgroundColor)
    : GuiElement(id, gui, backgroundColor), element(element) {
    element->parent = this;
}

void ContainerItem::render() const {
    GuiElement::render();

    element->render();
}

Container::Container(const std::string& id, Gui* gui, const glm::vec4& backgroundColor)
    : GuiElement(id, gui, backgroundColor) {
}

void Container::addChild(GuiElement* child) {
    std::stringstream ss;
    ss << id << "Item" << children.size();

    ContainerItem* item = new ContainerItem(ss.str(), gui, child);
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