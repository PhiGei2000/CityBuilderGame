#pragma once

#include "misc/typedefs.hpp"

#include "objectLoader.hpp"

#include <string>
#include <typeindex>
#include <unordered_map>

class ResourceManager {
  private:
    struct ResourceHolder {
        std::type_index type = std::type_index(typeid(void));

        std::shared_ptr<void> data;
    };

    std::unordered_map<std::string, ResourceHolder> resources;

    ObjectLoader objectLoader;

    template<typename T>
    inline void setResource(const std::string& id, ResourcePtr<T> data) {
      ResourcePtr<void> dataPtr = ResourcePtr<T>(data);

      if (resources.contains(id)) {
        resources[id].data.swap(dataPtr);
      }
      else {
        resources[id] = ResourceHolder{std::type_index(typeid(T)), dataPtr};
      }
    }

  public:
    const std::string resourceDir;

    struct ResourceTypeException : public std::exception {
      private:
        const char* message;

      public:
        ResourceTypeException(const char* message);

        const char* what() const noexcept override;
    };

    ResourceManager(const std::string& resourceDir);

    void loadResources();

    template<typename T, typename... TArgs>
    void loadResource(const std::string& resourceId, const std::string& filename, TArgs... args);

    template<typename T>
    inline ResourcePtr<T> getResource(const std::string& resourceId) const {
      const std::type_index type = std::type_index(typeid(T));
      const ResourceHolder& resource = resources.at(resourceId);

      if (type != resource.type) {
        std::string message = "Resource could not converted to ";

        throw ResourceTypeException(message.append(type.name()).c_str());
      }

      return std::reinterpret_pointer_cast<T>(resource.data);
    }
};