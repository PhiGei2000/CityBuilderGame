#pragma once
#include "system.hpp"

#include "components/streetComponent.hpp"

#include "misc/streetGeometryGenerator.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <glm/glm.hpp>
#include <queue>
#include <unordered_map>

struct BuildEvent;

class StreetSystem : public System {
  protected:
    static constexpr glm::ivec2 offsets[4] = {
        glm::ivec2(1, 0),
        glm::ivec2(0, 1),
        glm::ivec2(-1, 0),
        glm::ivec2(0, -1)};

    virtual void init() override;

    bool streetGeometryOutdated = true;
    entt::entity streetEntity;

    glm::ivec2 streetBuildStart = glm::ivec2{0};
    struct StreetBuildData {
        glm::ivec2 start, end;
    };

    std::queue<StreetBuildData> buildData;

    StreetGeometryGenerator geometryGenerator;

  public:
    StreetSystem(Game* game);

    virtual void update(float dt) override;

    void handle_buildEvent(const BuildEvent& e);
};