#pragma once
#include "misc/configuration.hpp"
#include "misc/roads/roadTypes.hpp"

#include <glm/glm.hpp>
#include <map>

struct RoadSpecs;
struct GeometryData;

using RoadPackGeometry = std::map<RoadType, GeometryData>;

class RoadGeometryGenerator {
  private:
    struct UVArea {
        float u, v;
        float sizeU, sizeV;
    };

    static constexpr float halfGrid = Configuration::gridSize / 2.0f;

    static constexpr UVArea roadwayUVArea = {0.125f, 0.0f, 0.25f, 0.5f};
    static constexpr UVArea halfRoadwayUVArea = {0.125f, 0.0f, 0.25f, 0.25f};
    static constexpr UVArea roadwayClearUVArea = {0.5f, 0.5f, 0.5f, 0.5f};
    static constexpr UVArea halfRoadwayClearUVArea = {0.5f, 0.5f, 0.25f, 0.25f};

    static constexpr UVArea sidewalkStraightUVAreas[] = {
        {  0.5f, 0.0f, 0.125f, 0.5f},
        {0.625f, 0.0f,  0.25f, 0.5f},
        {0.875f, 0.0f, 0.125f, 0.5f}
    };
    static constexpr UVArea halfSidewalkStraightUVAreas[] = {
        {  0.5f, 0.0f, 0.125f, 0.25f},
        {0.625f, 0.0f,  0.25f, 0.25f},
        {0.875f, 0.0f, 0.125f, 0.25f}
    };
    static constexpr UVArea sidewalkRoundUVAreas[] = {
        {  0.5f, 0.0f, 0.125f,   0.5f},
        {0.625f, 0.0f, 0.375f, 0.375f}
    };
    static constexpr UVArea crossingRoadwayUVArea = {0.0f, 0.5f, 0.5f, 0.5f};
    static constexpr UVArea tCrossingRoadwayUVArea = {0.125f, 0.5f, 0.375f, 0.5f};

    static GeometryData generateQuad(const glm::vec3& pos, const glm::vec3& first, const glm::vec3& second, const UVArea& uvs);

    static GeometryData generateAnnulus(const glm::vec3& center, float outerRadius, float innerRadius, unsigned int verticesCount, const UVArea& uvs);
    static GeometryData generateQuadCircle(const glm::vec3& center, float height, float radius, unsigned int verticesCount, const UVArea& uvs, bool normalsInside = false);

    static GeometryData generateAnnulusSector(const glm::vec3& center, float outerRadius, float innerRadius, unsigned int verticesCount, float startAngle, float endAngle, const UVArea& uvs);
    static GeometryData generateQuadCircleSector(const glm::vec3& center, float height, float radius, unsigned int verticesCount, float startAngle, float endAngle, const UVArea& uvs, bool normalsInside = false);

    static GeometryData generateNotConnected(const RoadSpecs& specs);
    static GeometryData generateStraight(const RoadSpecs& specs);
    static GeometryData generateCurve(const RoadSpecs& specs);
    static GeometryData generateTCrossing(const RoadSpecs& specs);
    static GeometryData generateCrossing(const RoadSpecs& specs);
    static GeometryData generateEnd(const RoadSpecs& specs);
    static GeometryData generateRamp(const RoadSpecs& specs);

  public:
    static RoadPackGeometry generateRoadPackGeometries(const RoadSpecs& specs);
};
