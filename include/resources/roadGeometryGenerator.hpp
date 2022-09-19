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
    static constexpr float halfGrid = Configuration::gridSize / 2.0f;

    static GeometryData generateQuad(const glm::vec3& pos, const glm::vec3& first, const glm::vec3& second);

    static GeometryData generateAnnulus(const glm::vec3& center, float outerRadius, float innerRadius, unsigned int verticesCount);
    static GeometryData generateQuadCircle(const glm::vec3& center, float height, float radius, unsigned int verticesCount, bool normalsInside = false);

    static GeometryData generateAnnulusSector(const glm::vec3& center, float outerRadius, float innerRadius, unsigned int verticesCount, float startAngle, float endAngle);
    static GeometryData generateQuadCircleSector(const glm::vec3& center, float height, float radius, unsigned int verticesCount, float startAngle, float endAngle, bool normalsInside = false);

    static GeometryData generateNotConnected(const RoadSpecs& specs);
    static GeometryData generateStraight(const RoadSpecs& specs);
    static GeometryData generateCurve(const RoadSpecs& specs);
    static GeometryData generateTCrossing(const RoadSpecs& specs);
    static GeometryData generateCrossing(const RoadSpecs& specs);
    static GeometryData generateEnd(const RoadSpecs& specs);

  public:
    static RoadPackGeometry generateRoadPackGeometries(const RoadSpecs& specs);
};
