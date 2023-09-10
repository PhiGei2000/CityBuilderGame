#pragma once
#include "container.hpp"

class StackPanel : public Container {
  public:
    enum class StackOrientation {
        ROW,
        COLUMN,
        ROW_REVERSE,
        COLUMN_REVERSE
    };

    enum class ItemAligment {
        BEGIN,
        END,
        CENTER,
        STRECH
    };

    StackOrientation orientation;
    ItemAligment itemAligment;
    float spacing = 30.0f;
    bool outerSpacing = true;

    StackPanel(const std::string& id, Gui* gui, StackOrientation orientation, const glm::vec4 backgroundColor, ItemAligment itemAligment = ItemAligment::CENTER);

    void setChildConstraints() override;

    Rectangle getBox() const override;
};
