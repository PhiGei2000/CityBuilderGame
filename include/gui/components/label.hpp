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

    std::string text;
    TextAlign textAlign;
    FontPtr font;
    int textSize;

    struct TextVertex {
        glm::vec2 position;
        glm::vec2 texCoord;

        static const VertexAttributes attributes;
    };

    virtual void applyConstraints() override;
    void updateTextGeometry();

  public:
    glm::vec4 textColor;

    Label(const std::string& id, Gui* gui, const glm::vec4& backgroundColor, const std::string& text, FontPtr font = nullptr, const int textSize = 24, TextAlign align = TextAlign::BEGIN, const glm::vec4& textColor = colors::white);

    // virtual void update() override;

    void setFont(FontPtr font);
    void setText(const std::string& text);
    void setTextAlign(TextAlign align);
    void setTextSize(int textSize);

    virtual void render() override;
};
