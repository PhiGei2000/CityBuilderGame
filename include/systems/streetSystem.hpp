#pragma once
#include "system.hpp"

#include "components/streetComponent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <glm/glm.hpp>
#include <unordered_map>

struct BuildEvent;

class StreetSystem : public System {
  protected:
    static constexpr glm::ivec2 offsets[4] = {
        glm::ivec2(1, 0),
        glm::ivec2(0, 1),
        glm::ivec2(-1, 0),
        glm::ivec2(0, -1)};

    std::unordered_map<glm::ivec2, entt::entity> streets;

    std::vector<entt::entity> outdatedStreets;
    std::vector<glm::ivec2> outdatedConnections;

    virtual void init() override;

    static StreetType getType(bool* connections);

    void createStreet(const glm::ivec2& gridPos);
  public:
    StreetSystem(Game* game);

    virtual void update(float dt) override;

    void handle_buildEvent(const BuildEvent& e);
};