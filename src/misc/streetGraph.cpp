#include "misc/streetGraph.hpp"

#include "misc/utility.hpp"

int StreetGraph::getNodeIndex(const glm::ivec2& position) const {
    const int nodesCount = nodes.size();
    // iterate through all nodes and return index if node at specified position is found
    for (int i = 0; i < nodesCount; i++) {
        if (nodes[i].gridPosition == position) {
            return i;
        }
    }

    // return -1 if no node was found
    return -1;
}

std::vector<int> StreetGraph::getEdges(const glm::ivec2& position) const {
    std::vector<int> result;

    // iterate through all edges
    for (int i = 0; i < edges.size(); i++) {
        const auto& edge = edges[i];

        // calculate edge length
        const glm::ivec2& start = nodes[edge.startIndex].gridPosition;
        const glm::ivec2& end = nodes[edge.endIndex].gridPosition;
        float edgeLength = static_cast<float>(glm::length(end - start));

        // search on edge if position is on edge
        for (int j = 0; j <= edgeLength; j++) {
            const glm::ivec2& edgePos = utility::interpolate(start, end, j / edgeLength);

            // check if position is on edge
            if (edgePos == position) {
                // add edge index to result and stop search on current edge
                result.push_back(i);
                break;
            }
        }
    }

    return result;
}

int StreetGraph::addNode(const glm::ivec2& position) {
    // check if position is occupied
    int index = getNodeIndex(position);
    if (index != -1) {
        // return found index if node already exists
        return index;
    }

    // create a new node at the end
    index = nodes.size();
    nodes.emplace_back(position);

    // return index of the created node
    return index;
}

void StreetGraph::removeNode(const glm::ivec2& position) {
    // get node index and check if node exists
    int index = getNodeIndex(position);
    if (index == -1)
        return;

    // remove node
    nodes.erase(nodes.begin() + index);

    // remove edges and update node indices
    for (auto it = edges.begin(); it != edges.end();) {
        auto& edge = *it;
        // connected to removed node
        if (edge.startIndex == index || edge.endIndex == index) {
            // delte edge
            it = edges.erase(it);
            continue;
        }
        
        // update edges with indices greater than removed node index
        if (edge.startIndex > index) {
            edge.startIndex--;
        }
        if (edge.endIndex > index) {
            edge.endIndex--;
        }

        it++;
    }
}

void StreetGraph::addEdge(const glm::ivec2& start, const glm::ivec2& end) {
    // check if edge is horizontal or vertical
    if (!(start.x == end.x || start.y == end.y)) {
        // insert node
        // TODO: Add mode to determinate building order (north/south first of east/west first)
        // north/south
        const glm::ivec2& nodePos{end.x, start.y};

        addEdge(start, nodePos);
        addEdge(nodePos, end);
    }
    else {
        // create start and end node if not exist
        int startIndex = addNode(start);
        int endIndex = addNode(end);

        edges.emplace_back(startIndex, endIndex);
    }
}
