#include "resources/roadGeometryGenerator.hpp"

#include "misc/roads/roadSpecs.hpp"
#include "rendering/geometryData.hpp"

#include <glm/glm.hpp>

RoadPackGeometry RoadGeometryGenerator::generateRoadPackGeometries(const RoadSpecs& specs) {
    RoadPackGeometry geometries;

    geometries[RoadType::NOT_CONNECTED] = generateNotConnected(specs);
    geometries[RoadType::EDGE] = generateStraight(specs);
    geometries[RoadType::CURVE] = generateCurve(specs);
    geometries[RoadType::T_CROSSING] = generateTCrossing(specs);
    geometries[RoadType::CROSSING] = generateCrossing(specs);
    geometries[RoadType::END] = generateEnd(specs);

    return geometries;
}

#pragma region Geometries

GeometryData RoadGeometryGenerator::generateQuad(const glm::vec3& position, const glm::vec3& first, const glm::vec3& second) {
    const glm::vec3& normal = glm::normalize(glm::cross(first, second));

    std::vector<Vertex> vertices = {
        Vertex(position, normal),
        Vertex(position + first, normal),
        Vertex(position + second, normal),
        Vertex(position + first + second, normal)};

    std::vector<unsigned int> indices = {
        0, 1, 2, 1, 3, 2};

    return GeometryData{vertices, indices};
}

GeometryData RoadGeometryGenerator::generateAnnulus(const glm::vec3& center, float outerRadius, float innerRadius, unsigned int verticesCount) {
    assert(verticesCount > 2);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float anglePerVertex = glm::radians(360.0f) / verticesCount;

    if (innerRadius == 0) {
        // circle
        vertices.emplace_back(Vertex(center, glm::vec3(0.0f, 1.0f, 0.0f)));

        // indices
        for (unsigned int i = 1; i <= verticesCount; i++) {
            indices.insert(indices.end(), {
                                              0,
                                              i,
                                              (i % verticesCount) + 1,
                                          });
        }
    }
    else {
        // annulus

        // inner vertices
        for (int i = 0; i < verticesCount; i++) {
            float x = innerRadius * glm::cos(i * anglePerVertex);
            float y = innerRadius * glm::sin(i * anglePerVertex);

            vertices.emplace_back(center + glm::vec3(x, 0.0f, y), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        // indices
        for (unsigned int i = 0; i < verticesCount; i++) {
            indices.insert(indices.end(), {verticesCount + i, i, verticesCount + (i + 1) % verticesCount,
                                           i, (i + 1) % verticesCount, verticesCount + (i + 1) % verticesCount});
        }
    }

    // outer vertices
    for (int i = 0; i < verticesCount; i++) {
        float x = outerRadius * glm::cos(i * anglePerVertex);
        float y = outerRadius * glm::sin(i * anglePerVertex);

        vertices.emplace_back(center + glm::vec3(x, 0.0f, y), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    return GeometryData{vertices, indices};
}

GeometryData RoadGeometryGenerator::generateQuadCircle(const glm::vec3& center, float height, float radius, unsigned int verticesCount, bool normalsInside) {
    assert(verticesCount > 2);
    assert(radius > 0);

    GeometryData data;

    const glm::vec3& heightVec = glm::vec3(0.0f, height, 0.0f);

    float anglePerVertex = glm::radians(360.0f) / verticesCount;

    for (int i = 0; i < verticesCount; i++) {

        float x = radius * glm::cos(i * anglePerVertex);
        float z = radius * glm::sin(i * anglePerVertex);

        float x2 = radius * glm::cos((i + 1) * anglePerVertex);
        float z2 = radius * glm::sin((i + 1) * anglePerVertex);

        if (normalsInside) {
            data.addData(generateQuad(center + glm::vec3(x, 0.0f, z), glm::vec3(x2 - x, 0.0f, z2 - z), heightVec));
        }
        else {
            data.addData(generateQuad(center + glm::vec3(x, 0.0f, z), heightVec, glm::vec3(x2 - x, 0.0f, z2 - z)));
        }
    }

    return data;
}

GeometryData RoadGeometryGenerator::generateAnnulusSector(const glm::vec3& center, float outerRadius, float innerRadius, unsigned int verticesCount, float startAngleDeg, float endAngleDeg) {
    assert(verticesCount > 2);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float startAngle = glm::radians(startAngleDeg);
    float anglePerVertex = glm::radians(endAngleDeg - startAngleDeg) / (verticesCount - 1);

    if (innerRadius == 0) {
        // circle
        vertices.emplace_back(Vertex(center, glm::vec3(0.0f, 1.0f, 0.0f)));

        // indices
        for (unsigned int i = 1; i <= verticesCount; i++) {
            indices.insert(indices.end(), {
                                              0,
                                              i,
                                              (i % verticesCount) + 1,
                                          });
        }
    }
    else {
        // annulus

        // inner vertices
        for (int i = 0; i < verticesCount; i++) {
            float x = innerRadius * glm::cos(startAngle + i * anglePerVertex);
            float y = innerRadius * glm::sin(startAngle + i * anglePerVertex);

            vertices.emplace_back(center + glm::vec3(x, 0.0f, y), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        // indices
        for (unsigned int i = 0; i < verticesCount - 1; i++) {
            indices.insert(indices.end(), {verticesCount + i, i, verticesCount + (i + 1) % verticesCount,
                                           i, (i + 1) % verticesCount, verticesCount + (i + 1) % verticesCount});
        }
    }

    // outer vertices
    for (int i = 0; i < verticesCount; i++) {
        float x = outerRadius * glm::cos(startAngle + i * anglePerVertex);
        float y = outerRadius * glm::sin(startAngle + i * anglePerVertex);

        vertices.emplace_back(center + glm::vec3(x, 0.0f, y), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    return GeometryData{vertices, indices};
}

GeometryData RoadGeometryGenerator::generateQuadCircleSector(const glm::vec3& center, float height, float radius, unsigned int verticesCount, float startAngleDeg, float endAngleDeg, bool normalsInside) {
    assert(verticesCount > 2);
    assert(radius > 0);

    GeometryData data;

    const glm::vec3& heightVec = glm::vec3(0.0f, height, 0.0f);

    float startAngle = glm::radians(startAngleDeg);
    float anglePerVertex = glm::radians(endAngleDeg - startAngleDeg) / (verticesCount - 1);

    for (int i = 0; i < verticesCount - 1; i++) {
        float x1 = radius * glm::cos(startAngle + i * anglePerVertex);
        float z1 = radius * glm::sin(startAngle + i * anglePerVertex);

        float x2 = radius * glm::cos(startAngle + (i + 1) * anglePerVertex);
        float z2 = radius * glm::sin(startAngle + (i + 1) * anglePerVertex);

        if (normalsInside) {
            data.addData(generateQuad(center + glm::vec3(x1, 0.0f, z1), glm::vec3(x2 - x1, 0.0f, z2 - z1), heightVec));
        }
        else {
            data.addData(generateQuad(center + glm::vec3(x1, 0.0f, z1), heightVec, glm::vec3(x2 - x1, 0.0f, z2 - z1)));
        }
    }

    return data;
}

#pragma endregion

#pragma region RoadTypes

GeometryData RoadGeometryGenerator::generateNotConnected(const RoadSpecs& specs) {
    GeometryData data;
    float sidewalkWidth = (Configuration::gridSize - specs.roadwayWidth) / 2.0f;

    // roadway
    data.addData(generateAnnulus(glm::vec3(0.0f, specs.roadwayHeight, 0.0f), specs.roadwayWidth / 2.0f, 0, 16));
    // sidewalk
    data.addData(generateAnnulus(glm::vec3(0.0f, specs.sidewalkHeight, 0.0f), halfGrid, specs.roadwayWidth / 2.0f, 16));
    data.addData(generateQuadCircle(glm::vec3(0.0f, specs.roadwayHeight, 0.0f), specs.sidewalkHeight - specs.roadwayHeight, specs.roadwayWidth / 2.0f, 16, true));
    data.addData(generateQuadCircle(glm::vec3(0.0f), specs.sidewalkHeight, halfGrid, 16));

    return data;
}

GeometryData RoadGeometryGenerator::generateStraight(const RoadSpecs& specs) {
    GeometryData data;
    float sidewalkWidth = (Configuration::gridSize - specs.roadwayWidth) / 2.0f;

    // left sidewalk
    data.addData(generateQuad(glm::vec3(-halfGrid, 0.0f, -halfGrid), glm::vec3(0.0f, specs.sidewalkHeight, 0.0f), glm::vec3(Configuration::gridSize, 0.0f, 0.0f)));
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.sidewalkHeight, -halfGrid), glm::vec3(0.0f, 0.0f, sidewalkWidth), glm::vec3(Configuration::gridSize, 0.0f, 0.0f)));
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.sidewalkHeight, -halfGrid + sidewalkWidth), glm::vec3(0.0f, -(specs.sidewalkHeight - specs.roadwayHeight), 0.0f), glm::vec3(Configuration::gridSize, 0.0f, 0.0f)));

    // roadway
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.roadwayHeight, -specs.roadwayWidth / 2), glm::vec3(0.0f, 0.0f, specs.roadwayWidth), glm::vec3(Configuration::gridSize, 0.0f, 0.0f)));

    // right sidewalk
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.roadwayHeight, halfGrid - sidewalkWidth), glm::vec3(0.0f, specs.sidewalkHeight - specs.roadwayHeight, 0.0f), glm::vec3(Configuration::gridSize, 0.0f, 0.0f)));
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.sidewalkHeight, halfGrid - sidewalkWidth), glm::vec3(0.0f, 0.0f, sidewalkWidth), glm::vec3(Configuration::gridSize, 0.0f, 0.0f)));
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.sidewalkHeight, halfGrid), glm::vec3(0.0f, -specs.sidewalkHeight, 0.0f), glm::vec3(Configuration::gridSize, 0.0f, 0.0f)));

    return data;
}

GeometryData RoadGeometryGenerator::generateCurve(const RoadSpecs& specs) {
    GeometryData data;
    float sidewalkWidth = (Configuration::gridSize - specs.roadwayWidth) / 2.0f;

    // inner sidewalk
    data.addData(generateAnnulusSector(glm::vec3(halfGrid, specs.sidewalkHeight, halfGrid), sidewalkWidth, 0, 4, 180, 270));
    data.addData(generateQuadCircleSector(glm::vec3(halfGrid, specs.roadwayHeight, halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, 4, 180, 270));

    // roadway
    data.addData(generateAnnulusSector(glm::vec3(halfGrid, specs.roadwayHeight, halfGrid), specs.roadwayWidth + sidewalkWidth, sidewalkWidth, 4, 180, 270));

    // outer sidewalk
    data.addData(generateQuadCircleSector(glm::vec3(halfGrid, specs.roadwayHeight, halfGrid), specs.sidewalkHeight - specs.roadwayHeight, specs.roadwayWidth + sidewalkWidth, 4, 180, 270, true));
    data.addData(generateAnnulusSector(glm::vec3(halfGrid, specs.sidewalkHeight, halfGrid), Configuration::gridSize, specs.roadwayWidth + sidewalkWidth, 4, 180, 270));
    data.addData(generateQuadCircleSector(glm::vec3(halfGrid, 0.0f, halfGrid), specs.sidewalkHeight, Configuration::gridSize, 4, 180, 270));

    return data;
}

GeometryData RoadGeometryGenerator::generateTCrossing(const RoadSpecs& specs) {
    GeometryData data;
    float sidewalkWidth = (Configuration::gridSize - specs.roadwayWidth) / 2.0f;

    // long sidewalk
    data.addData(generateQuad(glm::vec3(-halfGrid, 0.0f, -halfGrid), glm::vec3(0.0f, specs.sidewalkHeight, 0.0f), glm::vec3(Configuration::gridSize, 0.0f, 0.0f)));
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.sidewalkHeight, -halfGrid), glm::vec3(0.0f, 0.0f, sidewalkWidth), glm::vec3(Configuration::gridSize, 0.0f, 0.0f)));
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.sidewalkHeight, -halfGrid + sidewalkWidth), glm::vec3(0.0f, -(specs.sidewalkHeight - specs.roadwayHeight), 0.0f), glm::vec3(Configuration::gridSize, 0.0f, 0.0f)));

    // roadway
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.roadwayHeight, -halfGrid + sidewalkWidth), glm::vec3(0.0f, 0.0f, specs.roadwayWidth + sidewalkWidth), glm::vec3(Configuration::gridSize, 0.0f, 0.0f)));

    // short sidewalks
    data.addData(generateAnnulusSector(glm::vec3(halfGrid, specs.sidewalkHeight, halfGrid), sidewalkWidth, 0, 4, 180, 270));
    data.addData(generateQuadCircleSector(glm::vec3(halfGrid, specs.roadwayHeight, halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, 4, 180, 270));

    data.addData(generateAnnulusSector(glm::vec3(-halfGrid, specs.sidewalkHeight, halfGrid), sidewalkWidth, 0, 4, 270, 360));
    data.addData(generateQuadCircleSector(glm::vec3(-halfGrid, specs.roadwayHeight, halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, 4, 270, 360));

    return data;
}

GeometryData RoadGeometryGenerator::generateCrossing(const RoadSpecs& specs) {
    GeometryData data;
    float sidewalkWidth = (Configuration::gridSize - specs.roadwayWidth) / 2.0f;

    // roadway
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.roadwayHeight, -halfGrid), glm::vec3(0.0f, 0.0f, Configuration::gridSize), glm::vec3(Configuration::gridSize, 0.0f, 0.0f)));

    // sidewalks
    data.addData(generateAnnulusSector(glm::vec3(-halfGrid, specs.sidewalkHeight, -halfGrid), sidewalkWidth, 0, 4, 0, 90));
    data.addData(generateQuadCircleSector(glm::vec3(-halfGrid, specs.roadwayHeight, -halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, 4, 0, 90));

    data.addData(generateAnnulusSector(glm::vec3(halfGrid, specs.sidewalkHeight, -halfGrid), sidewalkWidth, 0, 4, 90, 180));
    data.addData(generateQuadCircleSector(glm::vec3(halfGrid, specs.roadwayHeight, -halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, 4, 90, 180));

    data.addData(generateAnnulusSector(glm::vec3(halfGrid, specs.sidewalkHeight, halfGrid), sidewalkWidth, 0, 4, 180, 270));
    data.addData(generateQuadCircleSector(glm::vec3(halfGrid, specs.roadwayHeight, halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, 4, 180, 270));

    data.addData(generateAnnulusSector(glm::vec3(-halfGrid, specs.sidewalkHeight, halfGrid), sidewalkWidth, 0, 4, 270, 360));
    data.addData(generateQuadCircleSector(glm::vec3(-halfGrid, specs.roadwayHeight, halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, 4, 270, 360));

    return data;
}

GeometryData RoadGeometryGenerator::generateEnd(const RoadSpecs& specs) {
    GeometryData data;
    float sidewalkWidth = (Configuration::gridSize - specs.roadwayWidth) / 2.0f;

    // left sidewalk
    data.addData(generateQuad(glm::vec3(halfGrid, 0.0f, -halfGrid), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, specs.sidewalkHeight, 0.0f)));
    data.addData(generateQuad(glm::vec3(halfGrid, specs.sidewalkHeight, -halfGrid), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, sidewalkWidth)));
    data.addData(generateQuad(glm::vec3(halfGrid, specs.sidewalkHeight, -halfGrid + sidewalkWidth), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, -(specs.sidewalkHeight - specs.roadwayHeight), 0.0f)));

    // roadway
    data.addData(generateQuad(glm::vec3(halfGrid, specs.roadwayHeight, -specs.roadwayWidth / 2), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, specs.roadwayWidth)));

    // right sidewalk
    data.addData(generateQuad(glm::vec3(halfGrid, specs.roadwayHeight, halfGrid - sidewalkWidth), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, specs.sidewalkHeight - specs.roadwayHeight, 0.0f)));
    data.addData(generateQuad(glm::vec3(halfGrid, specs.sidewalkHeight, halfGrid - sidewalkWidth), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, sidewalkWidth)));
    data.addData(generateQuad(glm::vec3(halfGrid, specs.sidewalkHeight, halfGrid), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, -specs.sidewalkHeight, 0.0f)));

    // end
    data.addData(generateAnnulusSector(glm::vec3(0.0f, specs.roadwayHeight, 0.0f), halfGrid - sidewalkWidth, 0, 8, 90, 270));
    data.addData(generateQuadCircleSector(glm::vec3(0.0f, specs.roadwayHeight, 0.0f), specs.sidewalkHeight - specs.roadwayHeight, halfGrid - sidewalkWidth, 8, 90, 270, true));
    data.addData(generateAnnulusSector(glm::vec3(0.0f, specs.sidewalkHeight, 0.0f), halfGrid, halfGrid - sidewalkWidth, 8, 90, 270));
    data.addData(generateQuadCircleSector(glm::vec3(0.0f, 0.0f, 0.0f), specs.sidewalkHeight, halfGrid, 8, 90, 270));

    return data;
}

#pragma endregion
