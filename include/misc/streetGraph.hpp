#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <vector>

enum class StreetGraphNodeType
{
    END,
    CURVE,
    T_CROSSING,
    CROSSING,
    EDGE_SEPARATOR
};

struct StreetGraphNode {
    StreetGraphNodeType type = StreetGraphNodeType::END;
    glm::ivec2 position;
};

struct StreetGraphEdge {
    StreetGraphNode *begin, *end;
};

struct StreetGraph {
  private:
    int getEdgeIndex(const glm::ivec2& position);
    StreetGraphNode* splitEdge(StreetGraphEdge& edge, const glm::ivec2& position);

    StreetGraphNode* getOrCreateNode(const glm::ivec2& position);
    StreetGraphNode* getOrCreateNode(int x, int y);

  public:
    std::vector<StreetGraphNode> nodes;
    std::vector<StreetGraphEdge> edges;

    void addEdge(const glm::ivec2& start, const glm::ivec2& end, bool xFirst = true);
};
