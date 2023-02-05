#include "resources/roadGeometryGenerator.hpp"

#include "misc/roads/roadSpecs.hpp"
#include "rendering/geometryData.hpp"

#include <glm/glm.hpp>

RoadPackGeometry RoadGeometryGenerator::generateRoadPackGeometries(const RoadSpecs& specs) {
    RoadPackGeometry geometries;

    geometries[RoadType::NOT_CONNECTED] = generateNotConnected(specs);
    geometries[RoadType::STRAIGHT] = generateStraight(specs);
    geometries[RoadType::CURVE] = generateCurve(specs);
    geometries[RoadType::T_CROSSING] = generateTCrossing(specs);
    geometries[RoadType::CROSSING] = generateCrossing(specs);
    geometries[RoadType::END] = generateEnd(specs);

    return geometries;
}

#pragma region Geometries

GeometryData RoadGeometryGenerator::generateQuad(const glm::vec3& position, const glm::vec3& first, const glm::vec3& second, const RoadGeometryGenerator::UVArea& area) {
    const glm::vec3& normal = glm::normalize(glm::cross(first, second));

    std::vector<Vertex> vertices = {
        Vertex(position, glm::vec2(area.u, area.v), normal),
        Vertex(position + first, glm::vec2(area.u + area.sizeU, area.v), normal),
        Vertex(position + second, glm::vec2(area.u, area.v + area.sizeV), normal),
        Vertex(position + first + second, glm::vec2(area.u + area.sizeU, area.v + area.sizeV), normal)};

    std::vector<unsigned int> indices = {
        0, 1, 2, 1, 3, 2};

    return GeometryData{vertices, indices};
}

GeometryData RoadGeometryGenerator::generateAnnulus(const glm::vec3& center, float outerRadius, float innerRadius, unsigned int verticesCount, const RoadGeometryGenerator::UVArea& uvs) {
    assert(verticesCount > 2);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float anglePerVertex = glm::radians(360.0f) / verticesCount;

    if (innerRadius == 0) {
        float uRadius = uvs.sizeU / 2.0f;
        float vRadius = uvs.sizeV / 2.0f;
        const glm::vec2& uvCenter = glm::vec2(uvs.u + uRadius, uvs.v + vRadius);

        // circle
        vertices.emplace_back(Vertex(center, uvCenter, glm::vec3(0.0f, 1.0f, 0.0f)));

        // outer vertices
        for (int i = 0; i <= verticesCount; i++) {
            float x = outerRadius * glm::cos(i * anglePerVertex);
            float y = outerRadius * glm::sin(i * anglePerVertex);

            const glm::vec2& uvOffset = glm::vec2(uRadius * glm::cos(i * anglePerVertex), vRadius * glm::sin(i * anglePerVertex));

            vertices.emplace_back(center + glm::vec3(x, 0.0f, y), uvCenter + uvOffset, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        // indices
        for (unsigned int i = 1; i <= verticesCount; i++) {
            indices.insert(indices.end(), {
                                              0,
                                              i,
                                              i + 1,
                                          });
        }
    }
    else {
        // annulus
        float vPerVertex = uvs.sizeV / verticesCount;

        // inner vertices
        for (int i = 0; i <= verticesCount; i++) {
            float x = innerRadius * glm::cos(i * anglePerVertex);
            float y = innerRadius * glm::sin(i * anglePerVertex);

            vertices.emplace_back(center + glm::vec3(x, 0.0f, y), glm::vec2(uvs.u, uvs.v + i * vPerVertex), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        // outer vertices
        for (int i = 0; i <= verticesCount; i++) {
            float x = outerRadius * glm::cos(i * anglePerVertex);
            float y = outerRadius * glm::sin(i * anglePerVertex);

            vertices.emplace_back(center + glm::vec3(x, 0.0f, y), glm::vec2(uvs.u + uvs.sizeU, uvs.v + i * vPerVertex), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        // indices
        for (unsigned int i = 0; i < verticesCount; i++) {
            indices.insert(indices.end(), {verticesCount + i, i, verticesCount + i + 1,
                                           i, i + 1, verticesCount + i + 1});
        }
    }

    return GeometryData{vertices, indices};
}

GeometryData RoadGeometryGenerator::generateQuadCircle(const glm::vec3& center, float height, float radius, unsigned int verticesCount, const UVArea& uvs, bool normalsInside) {
    assert(verticesCount > 2);
    assert(radius > 0);

    GeometryData data;

    float vPerVertex = uvs.sizeV / verticesCount;

    const glm::vec3& heightVec = glm::vec3(0.0f, height, 0.0f);

    float anglePerVertex = glm::radians(360.0f) / verticesCount;

    for (int i = 0; i < verticesCount; i++) {

        float x1 = radius * glm::cos(i * anglePerVertex);
        float z1 = radius * glm::sin(i * anglePerVertex);

        float x2 = radius * glm::cos((i + 1) * anglePerVertex);
        float z2 = radius * glm::sin((i + 1) * anglePerVertex);

        const UVArea& area = {uvs.u, uvs.v + i * vPerVertex, uvs.sizeU, vPerVertex};

        if (normalsInside) {
            data.addData(generateQuad(center + glm::vec3(x1, 0.0f, z1), glm::vec3(x2 - x1, 0.0f, z2 - z1), heightVec, area));
        }
        else {
            data.addData(generateQuad(center + glm::vec3(x1, 0.0f, z1), heightVec, glm::vec3(x2 - x1, 0.0f, z2 - z1), area));
        }
    }

    return data;
}

GeometryData RoadGeometryGenerator::generateAnnulusSector(const glm::vec3& center, float outerRadius, float innerRadius, unsigned int verticesCount, float startAngleDeg, float endAngleDeg, const UVArea& uvs) {
    assert(verticesCount > 2);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float startAngle = glm::radians(startAngleDeg);
    float anglePerVertex = glm::radians(endAngleDeg - startAngleDeg) / (verticesCount - 1);

    if (innerRadius == 0) {
        // circle
        float uRadius = uvs.sizeU / 2.0f;
        float vRadius = uvs.sizeV / 2.0f;
        const glm::vec2& uvCenter = glm::vec2(uvs.u + uRadius, uvs.v + vRadius);

        // outer vertices
        for (int i = 0; i < verticesCount - 1; i++) {
            float x1 = outerRadius * glm::cos(startAngle + (i + 1) * anglePerVertex);
            float y1 = outerRadius * glm::sin(startAngle + (i + 1) * anglePerVertex);

            float x2 = outerRadius * glm::cos(startAngle + i * anglePerVertex);
            float y2 = outerRadius * glm::sin(startAngle + i * anglePerVertex);

            const glm::vec2& uv1Offset = glm::vec2(uRadius * glm::cos(startAngle + (i + 1) * anglePerVertex), vRadius * glm::sin(startAngle + (i + 1) * anglePerVertex));
            const glm::vec2& uv2Offset = glm::vec2(uRadius * glm::cos(startAngle + i * anglePerVertex), vRadius * glm::sin(startAngle + i * anglePerVertex));

            vertices.emplace_back(Vertex(center, uvCenter, glm::vec3(0.0f, 1.0f, 0.0f)));
            vertices.emplace_back(center + glm::vec3(x1, 0.0f, y1), uvCenter + uv1Offset, glm::vec3(0.0f, 1.0f, 0.0f));
            vertices.emplace_back(center + glm::vec3(x2, 0.0f, y2), uvCenter + uv2Offset, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        // indices
        for (unsigned int i = 0; i < vertices.size(); i++) {
            indices.push_back(i);
        }
    }
    else {
        // annulus
        float vPerVertex = uvs.sizeV / (verticesCount - 1);

        // inner vertices
        for (int i = 0; i < verticesCount; i++) {
            float x = innerRadius * glm::cos(startAngle + i * anglePerVertex);
            float y = innerRadius * glm::sin(startAngle + i * anglePerVertex);

            vertices.emplace_back(center + glm::vec3(x, 0.0f, y), glm::vec2(uvs.u, uvs.v + i * vPerVertex), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        // outer vertices
        for (int i = 0; i < verticesCount; i++) {
            float x = outerRadius * glm::cos(startAngle + i * anglePerVertex);
            float y = outerRadius * glm::sin(startAngle + i * anglePerVertex);

            vertices.emplace_back(center + glm::vec3(x, 0.0f, y), glm::vec2(uvs.u + uvs.sizeU, uvs.v + i * vPerVertex), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        // indices
        for (unsigned int i = 0; i < verticesCount - 1; i++) {
            indices.insert(indices.end(), {verticesCount + i, i, verticesCount + (i + 1) % verticesCount,
                                           i, (i + 1) % verticesCount, verticesCount + (i + 1) % verticesCount});
        }
    }

    return GeometryData{vertices, indices};
}

GeometryData RoadGeometryGenerator::generateQuadCircleSector(const glm::vec3& center, float height, float radius, unsigned int verticesCount, float startAngleDeg, float endAngleDeg, const UVArea& uvs, bool normalsInside) {
    assert(verticesCount > 2);
    assert(radius > 0);

    GeometryData data;
    float vPerVertex = uvs.sizeV / (verticesCount - 1);

    const glm::vec3& heightVec = glm::vec3(0.0f, height, 0.0f);

    float startAngle = glm::radians(startAngleDeg);
    float anglePerVertex = glm::radians(endAngleDeg - startAngleDeg) / (verticesCount - 1);

    for (int i = 0; i < verticesCount - 1; i++) {
        float x1 = radius * glm::cos(startAngle + i * anglePerVertex);
        float z1 = radius * glm::sin(startAngle + i * anglePerVertex);

        float x2 = radius * glm::cos(startAngle + (i + 1) * anglePerVertex);
        float z2 = radius * glm::sin(startAngle + (i + 1) * anglePerVertex);

        const UVArea& area = {uvs.u, uvs.v + i * vPerVertex, uvs.sizeU, vPerVertex};

        if (normalsInside) {
            data.addData(generateQuad(center + glm::vec3(x1, 0.0f, z1), glm::vec3(x2 - x1, 0.0f, z2 - z1), heightVec, area));
        }
        else {
            data.addData(generateQuad(center + glm::vec3(x1, 0.0f, z1), heightVec, glm::vec3(x2 - x1, 0.0f, z2 - z1), area));
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
    data.addData(generateAnnulus(glm::vec3(0.0f, specs.roadwayHeight, 0.0f), specs.roadwayWidth / 2.0f, 0, specs.verticesPerCircle, roadwayClearUVArea));
    // sidewalk
    data.addData(generateQuadCircle(glm::vec3(0.0f), specs.sidewalkHeight, halfGrid, specs.verticesPerCircle, sidewalkStraightUVAreas[0]));
    data.addData(generateAnnulus(glm::vec3(0.0f, specs.sidewalkHeight, 0.0f), halfGrid, specs.roadwayWidth / 2.0f, specs.verticesPerCircle, sidewalkStraightUVAreas[1]));
    data.addData(generateQuadCircle(glm::vec3(0.0f, specs.roadwayHeight, 0.0f), specs.sidewalkHeight - specs.roadwayHeight, specs.roadwayWidth / 2.0f, specs.verticesPerCircle, sidewalkStraightUVAreas[2], true));

    return data;
}

GeometryData RoadGeometryGenerator::generateStraight(const RoadSpecs& specs) {
    GeometryData data;
    float sidewalkWidth = (Configuration::gridSize - specs.roadwayWidth) / 2.0f;

    // left sidewalk
    // outer side
    data.addData(generateQuad(glm::vec3(-halfGrid, 0.0f, -halfGrid), glm::vec3(0.0f, specs.sidewalkHeight, 0.0f), glm::vec3(Configuration::gridSize, 0.0f, 0.0f), sidewalkStraightUVAreas[0]));
    // top
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.sidewalkHeight, -halfGrid), glm::vec3(0.0f, 0.0f, sidewalkWidth), glm::vec3(Configuration::gridSize, 0.0f, 0.0f), sidewalkStraightUVAreas[1]));
    // inner side
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.sidewalkHeight, -halfGrid + sidewalkWidth), glm::vec3(0.0f, -(specs.sidewalkHeight - specs.roadwayHeight), 0.0f), glm::vec3(Configuration::gridSize, 0.0f, 0.0f), sidewalkStraightUVAreas[2]));

    // roadway
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.roadwayHeight, -specs.roadwayWidth / 2), glm::vec3(0.0f, 0.0f, specs.roadwayWidth), glm::vec3(Configuration::gridSize, 0.0f, 0.0f), roadwayUVArea));

    // right sidewalk
    // inner side
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.roadwayHeight, halfGrid - sidewalkWidth), glm::vec3(0.0f, specs.sidewalkHeight - specs.roadwayHeight, 0.0f), glm::vec3(Configuration::gridSize, 0.0f, 0.0f), sidewalkStraightUVAreas[2]));
    // top
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.sidewalkHeight, halfGrid - sidewalkWidth), glm::vec3(0.0f, 0.0f, sidewalkWidth), glm::vec3(Configuration::gridSize, 0.0f, 0.0f), sidewalkStraightUVAreas[1]));
    // outer side
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.sidewalkHeight, halfGrid), glm::vec3(0.0f, -specs.sidewalkHeight, 0.0f), glm::vec3(Configuration::gridSize, 0.0f, 0.0f), sidewalkStraightUVAreas[0]));

    return data;
}

GeometryData RoadGeometryGenerator::generateCurve(const RoadSpecs& specs) {
    GeometryData data;
    float sidewalkWidth = (Configuration::gridSize - specs.roadwayWidth) / 2.0f;

    // inner sidewalk
    data.addData(generateQuadCircleSector(glm::vec3(halfGrid, specs.roadwayHeight, halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, specs.verticesPerCircle / 4, 180, 270, sidewalkRoundUVAreas[0]));
    data.addData(generateAnnulusSector(glm::vec3(halfGrid, specs.sidewalkHeight, halfGrid), sidewalkWidth, 0, specs.verticesPerCircle / 4, 180, 270, sidewalkRoundUVAreas[1]));

    // roadway
    data.addData(generateAnnulusSector(glm::vec3(halfGrid, specs.roadwayHeight, halfGrid), specs.roadwayWidth + sidewalkWidth, sidewalkWidth, specs.verticesPerCircle / 4, 180, 270, roadwayUVArea));

    // outer sidewalk
    data.addData(generateQuadCircleSector(glm::vec3(halfGrid, 0.0f, halfGrid), specs.sidewalkHeight, Configuration::gridSize, specs.verticesPerCircle / 4, 180, 270, sidewalkStraightUVAreas[0]));
    data.addData(generateAnnulusSector(glm::vec3(halfGrid, specs.sidewalkHeight, halfGrid), Configuration::gridSize, specs.roadwayWidth + sidewalkWidth, specs.verticesPerCircle / 4, 180, 270, sidewalkStraightUVAreas[1]));
    data.addData(generateQuadCircleSector(glm::vec3(halfGrid, specs.roadwayHeight, halfGrid), specs.sidewalkHeight - specs.roadwayHeight, specs.roadwayWidth + sidewalkWidth, specs.verticesPerCircle / 4, 180, 270, sidewalkStraightUVAreas[2], true));

    return data;
}

GeometryData RoadGeometryGenerator::generateTCrossing(const RoadSpecs& specs) {
    GeometryData data;
    float sidewalkWidth = (Configuration::gridSize - specs.roadwayWidth) / 2.0f;

    // long sidewalk
    // outer side
    data.addData(generateQuad(glm::vec3(-halfGrid, 0.0f, -halfGrid), glm::vec3(0.0f, specs.sidewalkHeight, 0.0f), glm::vec3(Configuration::gridSize, 0.0f, 0.0f), sidewalkStraightUVAreas[0]));
    // top
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.sidewalkHeight, -halfGrid), glm::vec3(0.0f, 0.0f, sidewalkWidth), glm::vec3(Configuration::gridSize, 0.0f, 0.0f), sidewalkStraightUVAreas[1]));
    // inner side
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.sidewalkHeight, -halfGrid + sidewalkWidth), glm::vec3(0.0f, -(specs.sidewalkHeight - specs.roadwayHeight), 0.0f), glm::vec3(Configuration::gridSize, 0.0f, 0.0f), sidewalkStraightUVAreas[2]));

    // roadway
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.roadwayHeight, -halfGrid + sidewalkWidth), glm::vec3(0.0f, 0.0f, specs.roadwayWidth + sidewalkWidth), glm::vec3(Configuration::gridSize, 0.0f, 0.0f), tCrossingRoadwayUVArea));    

    // short sidewalks
    data.addData(generateQuadCircleSector(glm::vec3(halfGrid, specs.roadwayHeight, halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, specs.verticesPerCircle / 4, 180, 270, sidewalkRoundUVAreas[0]));
    data.addData(generateAnnulusSector(glm::vec3(halfGrid, specs.sidewalkHeight, halfGrid), sidewalkWidth, 0, specs.verticesPerCircle / 4, 180, 270, sidewalkRoundUVAreas[1]));

    data.addData(generateQuadCircleSector(glm::vec3(-halfGrid, specs.roadwayHeight, halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, specs.verticesPerCircle / 4, 270, 360, sidewalkRoundUVAreas[0]));
    data.addData(generateAnnulusSector(glm::vec3(-halfGrid, specs.sidewalkHeight, halfGrid), sidewalkWidth, 0, specs.verticesPerCircle / 4, 270, 360, sidewalkRoundUVAreas[1]));

    return data;
}

GeometryData RoadGeometryGenerator::generateCrossing(const RoadSpecs& specs) {
    GeometryData data;
    float sidewalkWidth = (Configuration::gridSize - specs.roadwayWidth) / 2.0f;

    // roadway    
    data.addData(generateQuad(glm::vec3(-halfGrid, specs.roadwayHeight, -halfGrid), glm::vec3(0.0f, 0.0f, Configuration::gridSize), glm::vec3(Configuration::gridSize, 0.0f, 0.0f), crossingRoadwayUVArea));

    // sidewalks
    data.addData(generateQuadCircleSector(glm::vec3(-halfGrid, specs.roadwayHeight, -halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, specs.verticesPerCircle / 4, 0, 90, sidewalkRoundUVAreas[0]));
    data.addData(generateAnnulusSector(glm::vec3(-halfGrid, specs.sidewalkHeight, -halfGrid), sidewalkWidth, 0, specs.verticesPerCircle / 4, 0, 90, sidewalkRoundUVAreas[1]));

    data.addData(generateQuadCircleSector(glm::vec3(halfGrid, specs.roadwayHeight, -halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, specs.verticesPerCircle / 4, 90, 180, sidewalkRoundUVAreas[0]));
    data.addData(generateAnnulusSector(glm::vec3(halfGrid, specs.sidewalkHeight, -halfGrid), sidewalkWidth, 0, specs.verticesPerCircle / 4, 90, 180, sidewalkRoundUVAreas[1]));

    data.addData(generateQuadCircleSector(glm::vec3(halfGrid, specs.roadwayHeight, halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, specs.verticesPerCircle / 4, 180, 270, sidewalkRoundUVAreas[0]));
    data.addData(generateAnnulusSector(glm::vec3(halfGrid, specs.sidewalkHeight, halfGrid), sidewalkWidth, 0, specs.verticesPerCircle / 4, 180, 270, sidewalkRoundUVAreas[1]));

    data.addData(generateQuadCircleSector(glm::vec3(-halfGrid, specs.roadwayHeight, halfGrid), specs.sidewalkHeight - specs.roadwayHeight, sidewalkWidth, specs.verticesPerCircle / 4, 270, 360, sidewalkRoundUVAreas[0]));
    data.addData(generateAnnulusSector(glm::vec3(-halfGrid, specs.sidewalkHeight, halfGrid), sidewalkWidth, 0, specs.verticesPerCircle / 4, 270, 360, sidewalkRoundUVAreas[1]));

    #if DEBUG
    data.save("streetCrossing.obj");
#endif

    return data;
}

GeometryData RoadGeometryGenerator::generateEnd(const RoadSpecs& specs) {
    GeometryData data;
    float sidewalkWidth = (Configuration::gridSize - specs.roadwayWidth) / 2.0f;

    // left sidewalk
    data.addData(generateQuad(glm::vec3(halfGrid, 0.0f, -halfGrid), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, specs.sidewalkHeight, 0.0f), halfSidewalkStraightUVAreas[0]));
    data.addData(generateQuad(glm::vec3(halfGrid, specs.sidewalkHeight, -halfGrid), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, sidewalkWidth), halfSidewalkStraightUVAreas[1]));
    data.addData(generateQuad(glm::vec3(halfGrid, specs.sidewalkHeight, -halfGrid + sidewalkWidth), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, -(specs.sidewalkHeight - specs.roadwayHeight), 0.0f), halfSidewalkStraightUVAreas[2]));

    // roadway
    data.addData(generateQuad(glm::vec3(0.0f, specs.roadwayHeight, -specs.roadwayWidth / 2), glm::vec3(0.0f, 0.0f, specs.roadwayWidth), glm::vec3(halfGrid, 0.0f, 0.0f), halfRoadwayUVArea));

    // right sidewalk
    data.addData(generateQuad(glm::vec3(halfGrid, specs.sidewalkHeight, halfGrid), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, -specs.sidewalkHeight, 0.0f), halfSidewalkStraightUVAreas[0]));
    data.addData(generateQuad(glm::vec3(halfGrid, specs.sidewalkHeight, halfGrid - sidewalkWidth), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, sidewalkWidth), halfSidewalkStraightUVAreas[1]));
    data.addData(generateQuad(glm::vec3(halfGrid, specs.roadwayHeight, halfGrid - sidewalkWidth), glm::vec3(-halfGrid, 0.0f, 0.0f), glm::vec3(0.0f, specs.sidewalkHeight - specs.roadwayHeight, 0.0f), halfSidewalkStraightUVAreas[2]));

    // end
    data.addData(generateAnnulusSector(glm::vec3(0.0f, specs.roadwayHeight, 0.0f), halfGrid - sidewalkWidth, 0, specs.verticesPerCircle / 2, 90, 270, halfRoadwayClearUVArea));
    data.addData(generateQuadCircleSector(glm::vec3(0.0f, 0.0f, 0.0f), specs.sidewalkHeight, halfGrid, specs.verticesPerCircle / 2, 90, 270, sidewalkStraightUVAreas[0]));
    data.addData(generateAnnulusSector(glm::vec3(0.0f, specs.sidewalkHeight, 0.0f), halfGrid, halfGrid - sidewalkWidth, specs.verticesPerCircle / 2, 90, 270, sidewalkStraightUVAreas[1]));
    data.addData(generateQuadCircleSector(glm::vec3(0.0f, specs.roadwayHeight, 0.0f), specs.sidewalkHeight - specs.roadwayHeight, halfGrid - sidewalkWidth, specs.verticesPerCircle / 2, 90, 270, sidewalkStraightUVAreas[2], true));    

    return data;
}

#pragma endregion
