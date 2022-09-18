#pragma once
#include "roadSection.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <map>
#include <unordered_map>

struct RoadSectionCollection {
  private:
    std::unordered_map<glm::ivec2, unsigned int> keyMap;
    std::map<unsigned int, RoadSection> roadSections;

    unsigned int currentKey = 0;

  public:
    struct iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = RoadSection;
        using pointer = RoadSection*;
        using reference = RoadSection&;
        using underlying_iterator = std::map<unsigned int, RoadSection>::iterator;

        iterator(underlying_iterator it);

        reference operator*() const;
        pointer operator->();
        iterator& operator++();
        friend bool operator==(const iterator& a, const iterator& b);
        friend bool operator!=(const iterator& a, const iterator& b);

      private:
        underlying_iterator m_it;
    };

    RoadSection& operator[](const glm::ivec2& position);

    RoadSection& at(const glm::ivec2& position);
    const RoadSection& at(const glm::ivec2& position) const;

    bool contains(const glm::ivec2& position) const;
    
    RoadSection& emplace(RoadSection&& section);

    iterator begin() noexcept;
    iterator end() noexcept;
};
