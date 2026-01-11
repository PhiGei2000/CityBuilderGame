#pragma once
#include "events/event.hpp"
#include "resources/resourceManager.hpp"
#include "gameState.hpp"

#include "misc/terrain.hpp"
#include "misc/typedefs.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <noise/noise.h>

#include <fstream>

class System;
class Application;


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
    const entt::registry& getRegistry() const;
    entt::dispatcher& getEventDispatcher();
    ResourceManager& getResourceManager();

    void update(float dt);
    void reloadResources();

    int getKey(int key) const;

    glm::vec2 getMousePos() const;
    int getMouseButton(int button) const;

    void setState(GameState state);
    GameState getState() const;

    template<bool ignoreState, EventType<ignoreState> Event>
    inline void raiseEvent(Event& event) {
        if (state != GameState::PAUSED || ignoreState) {
            eventDispatcher.trigger<Event&>(event);
        }
    }

#if DEBUG
    void log(const std::string& message);
#endif
};
