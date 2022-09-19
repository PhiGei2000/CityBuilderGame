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

    virtual void show() override;
    virtual void hide() override;

    void render() const override;

    virtual void setChildConstraints() = 0;

    void handleMouseButtonEvent(const MouseButtonEvent& e) override;
    void handleMouseMoveEvent(const MouseMoveEvent& e) override;
};
