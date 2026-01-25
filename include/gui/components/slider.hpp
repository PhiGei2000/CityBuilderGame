#pragma once
#include "gui/colors.hpp"
#include "gui/components/widget.hpp"

class Slider : public Widget {
  protected:
    bool mouseDown = false;

    Rectangle sliderBarArea;
    Rectangle indicatorArea;

    Rectangle getIndicatorBox() const;


    public:
    float minValue = 0.0f;
    float maxValue = 1.0f;
    float value = 0.0f;
    glm::vec4 color;

    Constraint sliderBarHeight;
    Constraint sliderBarWidth;
    AbsoluteConstraint sliderIndicatorSize;

    Slider(const std::string& id, Gui* gui, const glm::vec4& color = colors::white, const glm::vec4& backgroundColor = colors::transparent);

    virtual void applyConstraints() override;

    virtual void render() override;

    virtual void handleMouseButtonEvent(MouseButtonEvent& e) override;
    virtual void handleMouseMoveEvent(MouseMoveEvent& e) override;
};