#pragma once

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

class ResourceManager {
  private:
    struct Resource {
        std::type_index type = std::type_index(typeid(void*));

        std::shared_ptr<void> data;
    };

    std::unordered_map<std::string, Resource> resources;

  public:
    struct ResourceTypeException : public std::exception {
      private:
        const char* message;

      public:
        ResourceTypeException(const char* message);

        const char* what() const noexcept override;
    };

    template<typename T>
    void setResource(const std::string& resourceId, T* data);
    template<typename T>
    std::shared_ptr<T> getResource(const std::string& resourceId) const;
};