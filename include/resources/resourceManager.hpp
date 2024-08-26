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

#include "misc/typedefs.hpp"

#include "objectLoader.hpp"

#include <string>
#include <typeindex>
#include <unordered_map>
#include <filesystem>

class Application;

class ResourceManager {
  private:
    struct ResourceHolder {
        std::type_index type = std::type_index(typeid(void));

        std::shared_ptr<void> data;
    };

    std::unordered_map<std::string, ResourceHolder> resources;

    ObjectLoader objectLoader;
    const Application* app;

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
    const std::filesystem::path resourceDir;

    struct ResourceTypeException : public std::exception {
      private:
        const char* message;

      public:
        ResourceTypeException(const char* message);

        const char* what() const noexcept override;
    };

    ResourceManager(const std::string& resourceDir, const Application* app);

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

    template<typename T>
    inline std::unordered_map<std::string, ResourcePtr<T>> getResources() const {
        std::unordered_map<std::string, ResourcePtr<T>> resources;
        const std::type_index& type = std::type_index(typeid(T));

        auto it = this->resources.begin();
        while (it != this->resources.end()) {
            const auto& [key, data] = *it;
            if (data.type == type) {
                resources[key] = std::reinterpret_pointer_cast<T>(data.data);
            }

            it++;
        }

        return resources;
    }
};
