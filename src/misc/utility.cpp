#include "misc/utility.hpp"

namespace Utility {
    template<typename T>
    std::ostream& operator<<(std::ostream& os, const glm::vec<1, T>& vec) {
        return os << "x: " << vec.x;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const glm::vec<2, T>& vec) {
        return os << "x: " << vec.x << " y: " << vec.y;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const glm::vec<3, T>& vec) {
        return os << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z;
    }

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const glm::vec<4, T>& vec) {
        return os << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z << " w: " << vec.w;
    }
}