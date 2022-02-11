#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <array>
#include <unordered_map>
#include <vector>

#include <glm/gtx/hash.hpp>

#include "streetTypes.hpp"
#include "misc/direction.hpp"

struct StreetGraphNode {
    glm::ivec2 position;
    // int rotation;
    bool connections[4] = {false, false, false, false};
    StreetType type = StreetType::END;

    constexpr bool connected(Direction direction) const;
};

struct StreetGraphEdge {
    glm::ivec2 start, end;

    int length() const;
};

struct StreetGraph {
  private:
    int getEdge(const glm::ivec2& position);

    void splitEdge(int edgeIndex, const glm::ivec2& position);

    void createNode(const glm::ivec2& position);
    void createNode(int x, int y);

    static constexpr std::array<glm::ivec2, 4> getNeighborPositions(const glm::ivec2& position);

  public:
    std::unordered_map<glm::ivec2, StreetGraphNode> nodes;
    std::vector<StreetGraphEdge> edges;

    void addEdge(const glm::ivec2& start, const glm::ivec2& end, bool xFirst = true);

    void updateNodes();
};
