#include "misc/streetGeometryGenerator.hpp"

std::map<StreetGraphNodeType, GeometryData> StreetGeometryGenerator::nodeData = {
    std::make_pair(StreetGraphNodeType::END, ModelLoader::load("res/models/street_end.obj")),
    std::make_pair(StreetGraphNodeType::CURVE, ModelLoader::load("res/models/street_curve.obj")),
    std::make_pair(StreetGraphNodeType::T_CROSSING, ModelLoader::load("res/models/street_t_crossing.obj")),
    std::male_pair(StreetGraphNodeType::CROSSING, ModelLoader::load("res/models/street_crossing.obj"))};

GeometryData StreetGeometryGenerator::getNodeGeometry(const StreetGraphNode& node) {
}