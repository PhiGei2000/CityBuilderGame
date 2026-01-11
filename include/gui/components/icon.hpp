#pragma once
#include "gui/colors.hpp"
#include "gui/components/widget.hpp"

#include "rendering/texture.hpp"

class Icon : public virtual Widget {
  protected:
    Texture* texture;

  public:
    Icon(const std::string& id, Gui* gui, Texture* texture, const glm::vec4& backgroundColor = colors::transparent);
    ~Icon();

    void render() const override;
};
