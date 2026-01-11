#pragma once
#include "gui/colors.hpp"
#include "rendering/geometry.hpp"
#include "resources/font.hpp"
#include "widget.hpp"

#include <string>

enum class TextAlign {
    BEGIN,
    CENTER,
    END
};

class Label : public virtual Widget {
  protected:
    Geometry geometry;
    float textWidth, textHeight, baselineOffset = 0;

    struct TextVertex {
        glm::vec2 position;
        glm::vec2 texCoord;

        static const VertexAttributes attributes;
    };

  public:
    std::string text;
    TextAlign textAlign;
    glm::vec4 textColor;
    FontPtr font;
    int textSize;

    Label(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, FontPtr font = nullptr, const int textSize = 24, TextAlign align = TextAlign::BEGIN, const glm::vec4& textColor = colors::white);

    void updateTextGeometry();

    virtual void update() override;

    virtual Rectangle getBox() const override;

    virtual void render() const override;
};
