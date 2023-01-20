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
    Widget* getChild(const std::string& id) const;

    virtual void show() override;
    virtual void hide() override;

    void update() override;

    void render() const override;

    virtual void setChildConstraints() = 0;

    void handleMouseButtonEvent(MouseButtonEvent& e) override;
    void handleMouseMoveEvent(MouseMoveEvent& e) override;
};
