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
    std::unordered_map<glm::ivec2, entt::entity> streets;

    std::vector<entt::entity> outdatedStreets;
    std::vector<glm::ivec2> outdatedConnections;

    virtual void init() override;

    static StreetType getType(bool* connections);

  public:
    StreetSystem(Game* game);

    virtual void update(int dt) override;

    void handle_buildEvent(const BuildEvent& e);
};