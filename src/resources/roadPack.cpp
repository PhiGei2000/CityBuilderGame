#include "resources/roadPack.hpp"

#include "resources/roadGeometryGenerator.hpp"

RoadPack::RoadPack(const std::string& name, const RoadSpecs& specs, MaterialPtr material, ShaderPtr shader, const std::string& icon)
    : specs(specs), roadGeometries(new Mesh(shader)), name(name), icon(icon) {
    const auto& geometries = RoadGeometryGenerator::generateRoadPackGeometries(specs);

    for (const auto& [type, geometry] : geometries) {
        roadGeometries->geometries[getRoadTileTypeName(type)] = std::vector{std::make_pair(material, geometry)};
    }
}