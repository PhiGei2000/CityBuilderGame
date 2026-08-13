#pragma once
#include <queue>
#include <vector>

#include <glm/glm.hpp>

struct Path {
  protected:
    std::vector<glm::vec3> positions;
#if DEBUG
    friend struct RoadGraph;
#endif

  public:
    Path();
    Path(const std::initializer_list<glm::vec3>& positions);

    int length() const;

    void add(const glm::vec3& position);
    void join(const Path& other);
    void removeFirst(int count = 1);

    const glm::vec3& operator[](int index) const;
};

struct CarPath : public Path {
    std::queue<glm::ivec2> roadNodes;

    /// @brief Returns the current segment of the path
    /// @return Start, end and connection vector (end - start) of the first path segment
    std::tuple<glm::vec3, glm::vec3, glm::vec3> getCurrentSegment() const;

    std::tuple<glm::ivec2, glm::ivec2> getPathEndTile() const;
};