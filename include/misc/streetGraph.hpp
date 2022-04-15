#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include <array>
#include <unordered_map>
#include <vector>

#include <glm/gtx/hash.hpp>

#include "misc/direction.hpp"
#include "streetTypes.hpp"

struct StreetGraphNode {
    glm::ivec2 gridPosition;
};

struct StreetGraphEdge {
    int startIndex, endIndex;
};

struct StreetGraph {
  private:
    // set of nodes
    std::vector<StreetGraphNode> nodes;
    // set of edges
    std::vector<StreetGraphEdge> edges;

  public:
    // Returns the index of the node at the specified position or -1, if no node is found.
    int getNodeIndex(const glm::ivec2& position) const;

    // Returns the indcies of the edges at the specified position.
    std::vector<int> getEdges(const glm::ivec2& position) const;

    // Creates a new node at the specified position and returns the index of the created node. Does nothing if there is a node already.
    int addNode(const glm::ivec2& position);

    // Removes the node at the specified position and update edges
    void removeNode(const glm::ivec2& position);

    // Creates a new edge with endpoints at the specified positions. Creates new nodes at start- and endpoint if neccessary. Also creates nodes to create north/south and east/west edges only.
    void addEdge(const glm::ivec2& start, const glm::ivec2& end);

};
