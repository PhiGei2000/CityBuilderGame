#include "misc/ray.hpp"

#include "misc/coordinateTransform.hpp"

Ray::Ray(const glm::vec3& start, const glm::vec3& direction)
    : start(start), direction(glm::normalize(direction)) {
}

Ray Ray::fromPoints(const glm::vec3& start, const glm::vec3& p) {
    const glm::vec3& direction = p - start;
    return Ray(start, direction);
}

std::vector<std::pair<glm::ivec2, glm::vec3>> Ray::getCellIntersections(float maxLength) const {
    std::vector<std::pair<glm::ivec2, glm::vec3>> cells;
    const auto startPos = utility::worldToNormalizedWorldGridCoords(start);
    if (direction.x == 0.0f && direction.z == 0.0f) {
        cells.emplace_back(startPos, glm::vec3(static_cast<float>(INT_MAX)));
        return cells;
    }

    float lambda;
    int nextX = startPos.x, nextY = startPos.y;
    int xStep = -1, yStep = -1;
    glm::ivec2 currentCell = startPos;

    if (direction.x > 0) {
        nextX += 1;
        xStep = 1;
    }

    if (direction.z > 0) {
        nextY += 1;
        yStep = 1;
    }

    do {
        // determine next cell
        float lambdaX = direction.x == 0 ? static_cast<float>(INT_MAX) : (nextX - startPos.x) / direction.x;
        float lambdaY = direction.z == 0 ? static_cast<float>(INT_MAX) : (nextY - startPos.y) / direction.z;

        if (lambdaY < lambdaX) {
            lambda = lambdaY;
            nextY += yStep;
            currentCell.y += yStep;
        }
        else {
            lambda = lambdaX;
            nextX += xStep;
            currentCell.x += xStep;
        }

        const glm::vec3& intersectionPoint = start + Configuration::cellSize * lambda * direction;

        cells.emplace_back(currentCell, intersectionPoint);
    } while (std::abs(lambda) < maxLength);
    return cells;
}