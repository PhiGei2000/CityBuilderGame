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
    void setResource(const std::string& id, ResourcePtr<T> data);

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
    ResourcePtr<T> getResource(const std::string& resourceId) const;
};