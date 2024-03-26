/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#include "system.hpp"

#include "events/events.hpp"
#include "rendering/shader.hpp"
#include "rendering/geometry.hpp"

#include <glm/glm.hpp>

class DebugSystem : public System {
  protected:    
    unsigned int vbo, vao;

    Geometry* axisGeo;

    entt::entity cameraEntity;
    entt::entity debugEntity;

    void init() override;

  public:
    DebugSystem(Game* game);
    ~DebugSystem();

    void update(float dt) override;

    void handleKeyEvent(const KeyEvent& e);
};
