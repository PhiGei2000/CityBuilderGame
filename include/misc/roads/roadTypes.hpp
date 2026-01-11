#pragma once
#include <memory>
#include <string>

// enum class RoadTypes : unsigned char {
//     BASIC_ROADS,
//     UNDEFINED
// };

// inline RoadTypes operator++(RoadTypes& t, int) {
//     if (t == RoadTypes::UNDEFINED) {
//         return RoadTypes::UNDEFINED;
//     }

//     RoadTypes prevValue = t;
//     t = static_cast<RoadTypes>(static_cast<unsigned char>(t) + 1);
//     return prevValue;
// }

// inline constexpr std::string getRoadTypeName(RoadTypes type) {
//     std::string roadTypeNames[] = {
//         "BASIC_ROADS",
//     };

//     return roadTypeNames[static_cast<int>(type)];
// }

struct RoadPack;
class ResourceManager;

std::shared_ptr<RoadPack> getRoadPack(const ResourceManager& resourceManager, const std::string& type);
