#include "misc/roads/graph.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <numeric>

#if DEBUG
#include "misc/configuration.hpp"

void RoadGraph::drawPaths() const {
    for (const auto& [edge, data] : getEdges()) {
        if (data.length() == 0) {
            continue;
        }

        std::vector<unsigned int> indices(data.positions.size());
        std::iota(indices.begin(), indices.end(), 0);
        pathGeometries->bufferData(data.positions, indices, GL_DYNAMIC_DRAW);
        pathGeometries->draw();
    }

    for (const auto& [node, data] : getNodes()) {
        for (const auto& direction : data.paths) {
            for (const auto& path : direction) {
                if (path.length() == 0) {
                    continue;
                }

                std::vector<unsigned int> indices(path.positions.size());
                std::iota(indices.begin(), indices.end(), 0);
                pathGeometries->bufferData(path.positions, indices, GL_DYNAMIC_DRAW);
                pathGeometries->draw();
            }
        }
    }
}

std::ostream& operator<<(std::ostream& os, const RoadGraph& graph) {
    glm::ivec2 position;
    const auto& nodes = graph.getNodes();

    for (position.y = 0; position.y < Configuration::cellsPerChunk; position.y++) {
        for (position.x = 0; position.x < Configuration::cellsPerChunk; position.x++) {
            if (nodes.contains(position)) {
                os << "N";
            }
            else if (graph.onEdge(position)) {
                os << "E";
            }
            else {
                os << " ";
            }
        }

        os << "\r\n";
    }

    return os;
}
#endif