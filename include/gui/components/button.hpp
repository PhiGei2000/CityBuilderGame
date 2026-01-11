#pragma once

#include "gui/components/icon.hpp"
#include "gui/components/label.hpp"
#include "gui/components/widget.hpp"

class Button : public virtual Widget {
  public:
    EventDispatcher<MouseButtonEvent> onClick;

    Button(const std::string& id, Gui* gui, const glm::vec4& backgroundColor);

    void handleMouseButtonEvent(MouseButtonEvent& e) override;
};

class TextButton : public Label, public Button {
  public:
    TextButton(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, FontPtr font = nullptr, int textSize = 24, TextAlign align = TextAlign::BEGIN, const glm::vec4& textColor = colors::white);
};

class IconButton : public Icon, public Button {
  public:
    IconButton(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, Texture* icon);
};
