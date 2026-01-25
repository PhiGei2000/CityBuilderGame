#pragma once
#include "gui/colors.hpp"
#include "gui/components/widget.hpp"

#include <concepts>
#include <vector>

class Container : public Widget {
  protected:
    std::vector<Widget*> children;

  public:
    virtual void applyConstraints() override;

    Container(const std::string& id, Gui* gui, const glm::vec4& backgroundColor);
    virtual void addChild(Widget* child);
    Widget* getChild(const std::string& id) const;

    virtual void show() override;
    virtual void hide() override;

    virtual void invalidate() override;

    void update() override;

    void render() override;

    virtual void setChildConstraints();

    void handleMouseButtonEvent(MouseButtonEvent& e) override;
    void handleMouseMoveEvent(MouseMoveEvent& e) override;
};

template<typename T>
concept ContainerType = std::derived_from<T, Container>;
