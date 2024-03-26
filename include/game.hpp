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
#include "resources/resourceManager.hpp"

#include "misc/terrain.hpp"
#include "misc/typedefs.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <noise/noise.h>

#include <fstream>

class System;
class Application;

enum class GameState {
    PAUSED,
    RUNNING,
    BUILD_MODE
};

class Game {
    std::vector<System*> systems;

    ResourceManager resourceManager;

    entt::registry registry;
    entt::dispatcher eventDispatcher;

    Application* app;

    void init();

    GameState state = GameState::RUNNING;

#if DEBUG
    std::ofstream logStream;
#endif

  public:
    entt::entity camera;
    entt::entity sun = entt::null;
    Terrain terrain;

    Game(Application* app);

    bool debugMode = false;

    entt::registry& getRegistry();
    entt::dispatcher& getEventDispatcher();
    ResourceManager& getResourceManager();

    void update(float dt);
    void reloadResources();

    int getKey(int key) const;

    glm::vec2 getMousePos() const;

    void setState(GameState state);
    GameState getState() const;

    template<typename Event>
    void raiseEvent(Event& args);

#if DEBUG
    void log(const std::string& message);
#endif
};
