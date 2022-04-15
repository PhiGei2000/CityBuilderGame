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
    glm::ivec2 position;
    // int rotation;
    bool connections[4] = {false, false, false, false};
    StreetType type = StreetType::END;

    constexpr bool connected(Direction direction) const;

    // relative node input positions
    static constexpr glm::vec2 nodeInputs[] = {
        glm::vec2{ 0.0f, 3.25f},
        glm::vec2{1.75f,  0.0f},
        glm::vec2{ 5.0f, 1.75f},
        glm::vec2{3.25f,  5.0f},
    };

    // relative node output positions
    static constexpr glm::vec2 nodeOutputs[] = {
        glm::vec2{ 5.0f, 3.25f},
        glm::vec2{1.75f,  5.0f},
        glm::vec2{ 0.0f, 1.75f},
        glm::vec2{3.25f,  0.0f},
    };
};

struct StreetGraphEdge {
    glm::ivec2 start, end;

    int length() const;
};

struct StreetGraph {
  private:
    int getEdge(const glm::ivec2& position) const;

    void splitEdge(int edgeIndex, const glm::ivec2& position);

    void createNode(const glm::ivec2& position);
    void createNode(int x, int y);

    static std::array<glm::ivec2, 4> getNeighborPositions(const glm::ivec2& position);

  public:
    std::unordered_map<glm::ivec2, StreetGraphNode> nodes;
    std::vector<StreetGraphEdge> edges;

    void addEdge(const glm::ivec2& start, const glm::ivec2& end, bool xFirst = true);

    StreetGraphEdge getEdge(const glm::ivec2& nodePosition, Direction direction) const;
    StreetGraphNode getNextNode(const glm::ivec2& position, Direction direction) const;

    void updateNodes();
};
