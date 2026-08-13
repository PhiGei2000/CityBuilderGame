#pragma once
#include "gui/components/label.hpp"

#include "events/event.hpp"

class Checkbox : public Label {
  protected:
    bool state;
    Rectangle checkboxArea;

    virtual void applyConstraints() override;

  public:
    struct CheckboxStateChangedEvent : public Event<true> {
        bool newState;
    };

    EventDispatcher<const CheckboxStateChangedEvent&> onStateChanged;

    glm::vec4 checkboxColor = colors::white;

    Checkbox(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, bool initialState, FontPtr font = nullptr, const int textSize = 24, TextAlign align = TextAlign::BEGIN, const glm::vec4& textColor = colors::white);

    bool getState() const;

    virtual void render() override;

    virtual void handleMouseButtonEvent(MouseButtonEvent& e) override;
};
