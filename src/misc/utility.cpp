#include "misc/utility.hpp"

template<typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, const glm::vec<1, T, Q>& vec) {
    return os << "(" << vec.x << ")";
}

template<typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, const glm::vec<2, T, Q>& vec) {
    return os << "(" << vec.x << ", " << vec.y << ")";
}

template<typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, const glm::vec<3, T, Q>& vec) {
    return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

template<typename T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& os, const glm::vec<4, T, Q>& vec) {
    return os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
}

template std::ostream& operator<< <int, glm::packed_highp>(std::ostream&, const glm::vec<2, int, glm::packed_highp>&);
template std::ostream& operator<< <float, glm::packed_highp>(std::ostream&, const glm::vec<3, float, glm::packed_highp>&);
