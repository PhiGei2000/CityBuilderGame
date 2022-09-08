#pragma once
#include "../constraints.hpp"
#include "../rectangle.hpp"

#include "../eventDispatcher.hpp"

#include <string>

#include <glm/glm.hpp>

class Gui;
struct MouseMoveEvent;
struct MouseButtonEvent;

class Widget {
  protected:
    Gui* gui;
    bool visible = false;

    friend class Gui;

  public:
    const std::string id;

    EventDispatcher<MouseMoveEvent> onMouseEnter;
    EventDispatcher<MouseMoveEvent> onMouseLeave;

    Widget(const std::string& id, Gui* gui, const glm::vec4& backgroundColor);

    Widget* parent = nullptr;
    glm::vec4 backgroundColor;
    Constraints constraints;

    virtual void show();
    virtual void hide();
    bool isVisible() const;

    virtual void render() const;

    Rectangle getBox() const;

    virtual void handleMouseButtonEvent(const MouseButtonEvent& e);
    virtual void handleMouseMoveEvent(const MouseMoveEvent& e);
};
