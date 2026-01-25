#pragma once
#include "gui/components/container.hpp"

class StackPanel : public Container {
  protected:
    void applyConstraints() override;

    float calculateSpace(float totalSpace, float totalSpaceing, const std::function<float(Widget*)>& getChildSize, const std::function<Constraint&(Widget*)>& getChildSizeConstraint);
    void setChildPositions(float start, const std::function<float(Widget*, float)>& setChildPosition, bool reverse);

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
    };

    StackOrientation orientation;
    ItemAligment itemAligment;
    float spacing = 30.0f;

    StackPanel(const std::string& id, Gui* gui, StackOrientation orientation, const glm::vec4 backgroundColor, ItemAligment itemAligment = ItemAligment::CENTER);

    void setChildConstraints() override;
};
