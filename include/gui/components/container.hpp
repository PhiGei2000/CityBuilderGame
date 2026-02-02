#pragma once
#include "gui/colors.hpp"
#include "gui/components/widget.hpp"

#include <concepts>
#include <vector>

struct Padding {
    Constraint top = AbsoluteConstraint(0);
    Constraint left = AbsoluteConstraint(0);
    Constraint bottom = AbsoluteConstraint(0);
    Constraint right = AbsoluteConstraint(0);

    inline void set(const Constraint& top, const Constraint& left, const Constraint& bottom, const Constraint& right) {
        this->top = top;
        this->left = left;
        this->bottom = bottom;
        this->right = right;
    }

    inline void setHorizontal(const Constraint& horizontal) {
        left = horizontal;
        right = horizontal;
    }

    inline void setVertical(const Constraint& vertical) {
        top = vertical;
        bottom = vertical;
    }
};

class Container : public Widget {
  protected:
    std::vector<Widget*> children;

  public:
    Padding padding;

    virtual void applyConstraints() override;
    virtual Rectangle getChildArea();

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
