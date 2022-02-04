#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <array>
#include <unordered_map>
#include <vector>

#include <glm/gtx/hash.hpp>

enum class StreetGraphNodeType
{
    END_NOT_CONNECTED,
    END,
    CURVE,
    T_CROSSING,
    CROSSING,
    EDGE
};

struct StreetGraphNode {
    // StreetGraphNodeType type = StreetGraphNodeType::END;
    glm::ivec2 position;
    // int rotation;
    bool connections[4] = {false, false, false, false};
};

struct StreetGraphEdge {
    glm::ivec2 start, end;
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
