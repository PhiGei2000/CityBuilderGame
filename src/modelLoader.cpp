#include "modelLoader.hpp"

#include <iostream>
#include <sstream>

namespace trafficSimulation {
    glm::vec3 ModelLoader::parseVec3(const std::string& str, int startIndex) {
        std::stringstream ss(str.substr(startIndex));
        float x = 0, y = 0, z = 0;
        ss >> x;
        ss >> y;
        ss >> z;

        return glm::vec3(x, y, z);
    }

    glm::vec2 ModelLoader::parseVec2(const std::string& str, int startIndex) {
        std::stringstream ss(str.substr(startIndex));
        float x = 0, y = 0;
        ss >> x;
        ss >> y;

        return glm::vec2(x, y);
    }

    Geometry ModelLoader::load(const std::string& filename) {
        
    }
} // namespace trafficSimulation