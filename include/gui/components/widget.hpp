#pragma once
#include "gui/constraints.hpp"
#include "gui/rectangle.hpp"

#include "gui/eventDispatcher.hpp"

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
    Rectangle box;
    bool invalid = true;

  public:
    virtual void applyConstraints();

    const std::string id;

    EventDispatcher<MouseMoveEvent> onMouseEnter;
    EventDispatcher<MouseMoveEvent> onMouseLeave;

    Widget(const std::string& id, Gui* gui, const glm::vec4& backgroundColor);

    Widget* parent = nullptr;
    glm::vec4 backgroundColor;
    float cornerRadius = 0.0f;
    Constraints constraints;

    virtual void show();
    virtual void hide();
    bool isVisible() const;

    inline virtual void invalidate() {
        invalid = true;
    }

    virtual void update();

    virtual void render();

    virtual Rectangle getBox() {
        if (invalid) {
            applyConstraints();
        }

        return box;
    }

    virtual void handleMouseButtonEvent(MouseButtonEvent& e);
    virtual void handleMouseMoveEvent(MouseMoveEvent& e);
};

template<typename T>
concept WidgetType = std::derived_from<T, Widget>;
