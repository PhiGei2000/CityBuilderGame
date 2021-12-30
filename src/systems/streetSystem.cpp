#include "systems/streetSystem.hpp"

#include "components/components.hpp"
#include "events/events.hpp"

#include "misc/configuration.hpp"

StreetSystem::StreetSystem(Game* game)
    : System(game), streets{{}} {
    init();

    streets.clear();

    eventDispatcher.sink<BuildEvent>()
        .connect<&StreetSystem::handle_buildEvent>(*this);
}

void StreetSystem::init() {
}

StreetType StreetSystem::getType(bool* connections) {
    byte type = 0;
    for (int i = 0; i < 4; i++) {
        if (connections[i]) {
            type |= 1 << i;
        }
    }

    return (StreetType)type;
}

void StreetSystem::update(int dt) {
    const ResourceManager& resourceManager = game->getResourceManager();

    for (auto it = outdatedStreets.begin(); it != outdatedStreets.end();) {
        const StreetComponent& street = registry.get<StreetComponent>(*it);
        MeshComponent& mesh = registry.get<MeshComponent>(*it);
        TransformationComponent& transform = registry.get<TransformationComponent>(*it);
        transform.setRotation(glm::vec3(0));

        std::shared_ptr<Geometry> geo;
        switch (street.type) {
        // end streets
        case StreetType::NOT_CONNECTED:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_NOT_CONNECTED_GEOMETRY");
            break;
        case StreetType::END_NORTH:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_END_GEOMETRY");
            break;
        case StreetType::END_EAST:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_END_GEOMETRY");
            transform.setRotation(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
            break;
        case StreetType::END_SOUTH:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_END_GEOMETRY");
            transform.setRotation(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
            break;
        case StreetType::END_WEST:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_END_GEOMETRY");
            transform.setRotation(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
            break;
        // staight streets
        case StreetType::VERTICAL:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_STRAIGHT_GEOMETRY");
            break;
        case StreetType::HORIZONTAL:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_STRAIGHT_GEOMETRY");
            transform.setRotation(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
            break;
        // curves
        case StreetType::CURVE_NORTH_EAST:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_CURVE_GEOMETRY");
            transform.setRotation(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
            break;
        case StreetType::CURVE_SOUTH_EAST:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_CURVE_GEOMETRY");
            transform.setRotation(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
            break;
        case StreetType::CURVE_SOUTH_WEST:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_CURVE_GEOMETRY");
            break;
        case StreetType::CURVE_NORTH_WEST:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_CURVE_GEOMETRY");
            transform.setRotation(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
            break;
        // t crossing streets
        case StreetType::CROSSING_HORIZONTAL_NORTH:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_T_CROSSING_GEOMETRY");
            transform.setRotation(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
            break;
        case StreetType::CROSSING_VERTICAL_EAST:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_T_CROSSING_GEOMETRY");
            break;
        case StreetType::CROSSING_HORIZONTAL_SOUTH:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_T_CROSSING_GEOMETRY");
            transform.setRotation(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
            break;
        case StreetType::CROSSING_VERTICAL_WEST:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_T_CROSSING_GEOMETRY");
            transform.setRotation(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
            break;
        // crossing
        case StreetType::CROSSING_HORIZONTAL_VERTICAL:
            mesh.geometry = resourceManager.getResource<Geometry>("STREET_CROSSING_GEOMETRY");
            break;
        }

        transform.calculateTransform();
        it = outdatedStreets.erase(it);
    }

    for (auto it = outdatedConnections.begin(); it != outdatedConnections.end();) {
        const glm::ivec2& pos = *it;
        auto jt = streets.find(pos);
        if (jt != streets.end()) {

            constexpr glm::ivec2 offsets[4] = {
                glm::ivec2(1, 0),
                glm::ivec2(0, 1),
                glm::ivec2(-1, 0),
                glm::ivec2(0, -1)};

            bool connections[4];
            for (int i = 0; i < 4; i++) {
                connections[i] = streets.find(pos + offsets[i]) != streets.end();
            }

            const auto entity = streets.at(pos);

            StreetComponent& streetComponent = registry.get<StreetComponent>(entity);
            streetComponent.type = getType(connections);

            outdatedStreets.push_back(entity);
        }

        it = outdatedConnections.erase(it);
    }
}

void StreetSystem::handle_buildEvent(const BuildEvent& e) {
    if (e.type == BuildingType::STREET) {
        const glm::ivec2& position = e.gridPosition;

        auto it = streets.find(position);

        // create new street
        if (it == streets.end()) {
            const ResourceManager& resourceManager = game->getResourceManager();

            auto entity = registry.create();

            registry.emplace<TransformationComponent>(entity,
                                                      glm::vec3(Configuration::gridSize * (position.x + 0.5f), 0.0f, Configuration::gridSize * (position.y + 0.5f)),
                                                      glm::quat(),
                                                      glm::vec3(1.0f));

            registry.emplace<MeshComponent>(entity,
                                            resourceManager.getResource<Geometry>("STREET_NOT_CONNECTED_GEOMETRY"),
                                            resourceManager.getResource<Shader>("MESH_SHADER"),
                                            resourceManager.getResource<Texture>("STREET_TEXTURE"));

            registry.emplace<StreetComponent>(entity, StreetType::NOT_CONNECTED);

            outdatedConnections.push_back(position);
            outdatedConnections.push_back(glm::ivec2(position.x - 1, position.y));
            outdatedConnections.push_back(glm::ivec2(position.x + 1, position.y));
            outdatedConnections.push_back(glm::ivec2(position.x, position.y - 1));
            outdatedConnections.push_back(glm::ivec2(position.x, position.y + 1));

            streets[position] = entity;
        }
    }
}
