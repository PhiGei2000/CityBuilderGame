#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <unordered_map>
#include <vector>

enum class StreetGraphNodeType
{
    END,
    END_NOT_CONNECTED,
    CURVE,
    T_CROSSING,
    CROSSING,
    EDGE_SEPARATOR
};

struct StreetGraphNode {
    StreetGraphNodeType type = StreetGraphNodeType::END;
    glm::ivec2 position;
    int rotation;
};

struct StreetGraphEdge {
    glm::ivec2 begin, end;
};

struct StreetGraph {
  private:
    int getEdgeIndex(const glm::ivec2& position);
    void splitEdge(StreetGraphEdge& edge, const glm::ivec2& position);

    void createNode(const glm::ivec2& position);
    void createNode(int x, int y);

  public:
    std::unordered_map<glm::ivec2, StreetGraphNode> nodes;
    std::vector<StreetGraphEdge> edges;

    void addEdge(const glm::ivec2& start, const glm::ivec2& end, bool xFirst = true);

    void updateNodes();
};
