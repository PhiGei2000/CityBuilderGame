/*  Copyright (C) 2024  Philipp Geil <https://github.com/PhiGei2000>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once
#include "misc/configuration.hpp"
#include "misc/roads/roadTile.hpp"
#include "rendering/geometry.hpp"

#include <glm/glm.hpp>
#include <map>

struct RoadSpecs;
struct GeometryData;

using RoadPackGeometry = std::map<RoadTileTypes, GeometryPtr>;

class RoadGeometryGenerator {
  private:
    struct UVArea {
        float u, v;
        float sizeU, sizeV;
    };

    static constexpr float halfPixelSize = 0.5f / 512.0f;
    static constexpr float halfGrid = Configuration::cellSize / 2.0f;

    static constexpr UVArea roadwayUVArea = {0.125f + halfPixelSize, 0.0f + halfPixelSize, 0.25f - 2 * halfPixelSize, 0.5f - 2 * halfPixelSize};
    static constexpr UVArea halfRoadwayUVArea = {0.125f + halfPixelSize, 0.0f + halfPixelSize, 0.25f - 2 * halfPixelSize, 0.25f - 2 * halfPixelSize};
    static constexpr UVArea roadwayClearUVArea = {0.5f + halfPixelSize, 0.5f + halfPixelSize, 0.5f - 2 * halfPixelSize, 0.5f - 2 * halfPixelSize};
    static constexpr UVArea halfRoadwayClearUVArea = {0.5f + halfPixelSize, 0.5f + halfPixelSize, 0.25f - 2 * halfPixelSize, 0.25f - 2 * halfPixelSize};

    static constexpr UVArea sidewalkStraightUVAreas[] = {
        {  0.5f + halfPixelSize, 0.0f + halfPixelSize, 0.125f - 2 * halfPixelSize, 0.5f - 2 * halfPixelSize},
        {0.625f + halfPixelSize, 0.0f + halfPixelSize,  0.25f - 2 * halfPixelSize, 0.5f - 2 * halfPixelSize},
        {0.875f + halfPixelSize, 0.0f + halfPixelSize, 0.125f - 2 * halfPixelSize, 0.5f - 2 * halfPixelSize}
    };
    static constexpr UVArea halfSidewalkStraightUVAreas[] = {
        {  0.5f + halfPixelSize, 0.0f + halfPixelSize, 0.125f - 2 * halfPixelSize, 0.25f - 2 * halfPixelSize},
        {0.625f + halfPixelSize, 0.0f + halfPixelSize,  0.25f - 2 * halfPixelSize, 0.25f - 2 * halfPixelSize},
        {0.875f + halfPixelSize, 0.0f + halfPixelSize, 0.125f - 2 * halfPixelSize, 0.25f - 2 * halfPixelSize}
    };
    static constexpr UVArea sidewalkRoundUVAreas[] = {
        {  0.5f + halfPixelSize, 0.0f + halfPixelSize, 0.125f - 2 * halfPixelSize,   0.5f - 2 * halfPixelSize},
        {0.625f + halfPixelSize, 0.0f + halfPixelSize, 0.375f - 2 * halfPixelSize, 0.375f - 2 * halfPixelSize}
    };
    static constexpr UVArea crossingRoadwayUVArea = {0.0f + halfPixelSize, 0.5f + halfPixelSize, 0.5f- 2 * halfPixelSize, 0.5f - 2 * halfPixelSize};
    static constexpr UVArea tCrossingRoadwayUVArea = {0.125f + halfPixelSize, 0.5f + halfPixelSize, 0.375f- 2 * halfPixelSize, 0.5f - 2 * halfPixelSize};

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
