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
#include "game.hpp"

#include "application.hpp"
#include "components/components.hpp"
#include "events/events.hpp"
#include "misc/coordinateTransform.hpp"
#include "misc/utility.hpp"
#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "systems/systems.hpp"

Game::Game(Application* app)
    : app(app), resourceManager("res/"), terrain(this) {
    logStream = std::ofstream("log.txt");

    init();
}

void Game::init() {
    // init camera system
    systems.push_back(new CameraSystem(this));
    // entities
    camera = registry.view<CameraComponent>().front();
    sun = registry.create();

    // init other systems
    systems.push_back(new BuildSystem(this));
    systems.push_back(new TerrainSystem(this));
    systems.push_back(new RoadSystem(this));
    systems.push_back(new CarSystem(this));
    systems.push_back(new EnvironmentSystem(this));
    systems.push_back(new PhysicsSystem(this));
    systems.push_back(new DebugSystem(this));
    systems.push_back(new RenderSystem(this));
}

entt::registry& Game::getRegistry() {
    return registry;
}

entt::dispatcher& Game::getEventDispatcher() {
    return eventDispatcher;
}

ResourceManager& Game::getResourceManager() {
    return resourceManager;
}

void Game::update(float dt) {
    if (state == GameState::PAUSED) {
        // update render system
        systems.back()->update(dt);
    }
    else {
        for (System* system : systems) {
            system->update(dt);
        }
    }
}

void Game::reloadResources() {
    resourceManager.loadResources();
}

int Game::getKey(int key) const {
    return glfwGetKey(app->getWindow(), key);
}

glm::vec2 Game::getMousePos() const {
    double x, y;
    int width, height;
    glfwGetCursorPos(app->getWindow(), &x, &y);
    glfwGetFramebufferSize(app->getWindow(), &width, &height);

    return glm::vec2((float)(2 * x) / width - 1.0f, 1.0f - (float)(2 * y) / height);
}

int Game::getMouseButton(int button) const {
    return glfwGetMouseButton(app->getWindow(), button);
}

void Game::setState(GameState state) {
    this->state = state;
}

GameState Game::getState() const {
    return state;
}

// handle framebuffer size and camera events even if the game is paused
template<>
void Game::raiseEvent<FramebufferSizeEvent>(FramebufferSizeEvent& e) {
    eventDispatcher.trigger<FramebufferSizeEvent&>(e);
}

template<>
void Game::raiseEvent<CameraUpdateEvent>(CameraUpdateEvent& e) {
    eventDispatcher.trigger<CameraUpdateEvent&>(e);
}

template<typename Event>
void Game::raiseEvent(Event& e) {
    if (state != GameState::PAUSED) {
        eventDispatcher.trigger<Event&>(e);
    }
}

template<>
void Game::raiseEvent(BuildEvent& e) {
    if (state != GameState::PAUSED) {
        eventDispatcher.trigger<BuildEvent&>(e);

        if (!e.valid) {
            app->getGui()->showWarning("Invalid build position!");
        }
        else {
            app->getGui()->hideWarning();
        }
    }
}

template void Game::raiseEvent<KeyEvent>(KeyEvent&);
template void Game::raiseEvent<MouseButtonEvent>(MouseButtonEvent&);
template void Game::raiseEvent<MouseMoveEvent>(MouseMoveEvent&);
template void Game::raiseEvent<MouseScrollEvent>(MouseScrollEvent&);
template void Game::raiseEvent<ChunkCreatedEvent>(ChunkCreatedEvent&);
template void Game::raiseEvent<ChunkDestroyedEvent>(ChunkDestroyedEvent&);
template void Game::raiseEvent<ChunkUpdatedEvent>(ChunkUpdatedEvent&);
template void Game::raiseEvent<EntityMoveEvent>(EntityMoveEvent&);

#if DEBUG
void Game::log(const std::string& message) {
    logStream << message << std::endl;
}
#endif
