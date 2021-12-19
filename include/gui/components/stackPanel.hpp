#pragma once
#include "container.hpp"

class StackPanel : public Container {
  public:
    enum class StackOrientation
    {
        ROW,
        COLUMN,
        ROW_REVERSE,
        COLUMN_REVERSE
    };

    StackOrientation orientation;

    StackPanel(Gui* gui, StackOrientation orientation, const glm::vec4 backgroundColor);    

    void setChildConstraints() override;
};
