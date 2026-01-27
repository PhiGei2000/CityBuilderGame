#include "gui/menus/graphicsMenu.hpp"

#include "gui/components/label.hpp"
#include "gui/components/slider.hpp"
#include "gui/gui.hpp"

#include "application.hpp"
#include "components/sunLightComponent.hpp"
#include "game.hpp"

GraphicsMenu::GraphicsMenu(Gui* gui)
    : Menu<StackPanel>("graphicsMenu", gui, StackOrientation::COLUMN, colors::transparent) {
    spacing = 0.0f;
    constraints.y = CenterConstraint();
    constraints.width = RelativeConstraint(0.6f);

    Label* sunLabel = new Label("graphicsMenu.sun", gui, colors::anthraziteGrey, "Sun");
    sunLabel->constraints.x = AbsoluteConstraint(0);
    sunLabel->constraints.height = AbsoluteConstraint(45.0f);
    sunLabel->constraints.width = RelativeConstraint(1.0f);
    addChild(sunLabel);

    auto createSliderWithLabel = [&](const std::string& subID, const std::string& labelText, float initialValue, float maxValue = 1.0f, float minValue = 0.0f) -> Slider* {
        StackPanel* row = new StackPanel("graphicsMenu." + subID, gui, StackOrientation::ROW, colors::transparent, ItemAligment::BEGIN);
        row->spacing = 0;
        row->constraints.x = AbsoluteConstraint(0.0f);
        row->constraints.height = AbsoluteConstraint(45.0f);
        row->constraints.width = RelativeConstraint(1.0f);

        Label* label = new Label("graphicsMenu." + subID + "Label", gui, colors::anthraziteGrey, labelText);
        label->constraints.x = AbsoluteConstraint(0);
        label->constraints.height = AbsoluteConstraint(45.0f);
        label->constraints.width = AbsoluteConstraint(45.0f);
        row->addChild(label);

        Slider* slider = new Slider("graphicsMenu." + subID + "Slider", gui, initialValue, maxValue, minValue, colors::white, colors::anthraziteGrey);
        slider->constraints.x = AbsoluteConstraint(45.0f);
        slider->constraints.height = AbsoluteConstraint(45.0f);
        slider->constraints.width = FlexConstraint(1.0f);
        row->addChild(slider);
        addChild(row);

        return slider;
    };

    Game* game = gui->getApp()->getGame();
    const SunLightComponent& sunLight = game->getRegistry().get<SunLightComponent>(game->sun);
    createSliderWithLabel("sunRed", "R", sunLight.diffuse.r)->onValueChanged += [this](ValueChangedEvent& e) {
        Game* game = this->gui->getApp()->getGame();
        SunLightComponent& sunLight = game->getRegistry().get<SunLightComponent>(game->sun);
        sunLight.diffuse.r = e.newValue;
    };

    createSliderWithLabel("sunGreen", "G", sunLight.diffuse.g)->onValueChanged += [this](ValueChangedEvent& e) {
        Game* game = this->gui->getApp()->getGame();
        SunLightComponent& sunLight = game->getRegistry().get<SunLightComponent>(game->sun);
        sunLight.diffuse.g = e.newValue;
    };

    createSliderWithLabel("sunBlue", "B", sunLight.diffuse.b)->onValueChanged += [this](ValueChangedEvent& e) {
        Game* game = this->gui->getApp()->getGame();
        SunLightComponent& sunLight = game->getRegistry().get<SunLightComponent>(game->sun);
        sunLight.diffuse.b = e.newValue;
    };
}
