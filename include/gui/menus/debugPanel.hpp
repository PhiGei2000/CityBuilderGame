#pragma once
#include "gui/components/stackPanel.hpp"
#include "gui/menus/menu.hpp"

class DebugPanel : public Menu<StackPanel> {
  public:
    DebugPanel(Gui* gui);

    void update() override;

    inline virtual constexpr std::optional<int> getKey() const override {
        return GLFW_KEY_F1;
    }

    inline virtual constexpr bool toggleOnKey() const override {
        return true;
    }
};
