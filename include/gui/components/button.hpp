#pragma once

#include "label.hpp"

class Button : public Label {
  public:
    EventDispatcher<MouseButtonEvent> onClick;

    Button(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text);

    void handleMouseButtonEvent(const MouseButtonEvent& e) override;
};
