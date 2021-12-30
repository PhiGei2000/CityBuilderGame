#include "resourceManager.hpp"

#include "rendering/geometry.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"

#include <stdexcept>

ResourceManager::ResourceTypeException::ResourceTypeException(const char* message)
    : message(message) {
}

const char* ResourceManager::ResourceTypeException::what() const noexcept {
    return message;
}

template<typename T>
void ResourceManager::setResource(const std::string& resourceId, T* data) {
    const std::type_index type = std::type_index(typeid(T));

    resources[resourceId] = Resource{type, std::shared_ptr<T>(data)};
}

template<typename T>
std::shared_ptr<T> ResourceManager::getResource(const std::string& resourceId) const {
    const std::type_index type = std::type_index(typeid(T));
    const Resource& resource = resources.at(resourceId);

    if (type != resource.type) {
        std::string message = "Resource could not converted to ";

        throw ResourceTypeException(message.append(type.name()).c_str());
    }

    return std::reinterpret_pointer_cast<T>(resource.data);
}

template void ResourceManager::setResource<Texture>(const std::string&, Texture*);
template void ResourceManager::setResource<Shader>(const std::string&, Shader*);
template void ResourceManager::setResource<Geometry>(const std::string&, Geometry*);

template std::shared_ptr<Texture> ResourceManager::getResource<Texture>(const std::string&) const;
template std::shared_ptr<Shader> ResourceManager::getResource<Shader>(const std::string&) const;
template std::shared_ptr<Geometry> ResourceManager::getResource<Geometry>(const std::string&) const;
