#pragma once
#include "events/event.hpp"
#include "gameState.hpp"

struct GameStateChangedEvent : public Event<true> {
    GameState oldState;
    GameState newState;

    inline GameStateChangedEvent(GameState oldState, GameState newState) : oldState(oldState), newState(newState) {
    }
};