#include "misc/roads/roadTypes.hpp"

#include "resources/roadPack.hpp"
#include "resources/resourceManager.hpp"


RoadPackPtr getRoadPack(const ResourceManager& resourceManager, const std::string& type) {
    return resourceManager.getResource<RoadPack>(type);
}
