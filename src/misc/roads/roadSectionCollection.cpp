#include "misc/roads/roadSectionCollection.hpp"

#include <stdexcept>

RoadSectionCollection::iterator::iterator(underlying_iterator it)
    : m_it(it) {
}

RoadSectionCollection::iterator::reference RoadSectionCollection::iterator::operator*() const {
    return m_it->second;
}

RoadSectionCollection::iterator::pointer RoadSectionCollection::iterator::operator->() {
    return &(m_it->second);
}

RoadSectionCollection::iterator& RoadSectionCollection::iterator::operator++() {
    m_it++;
    return *this;
}

bool operator==(const RoadSectionCollection::iterator& a, const RoadSectionCollection::iterator& b) {
    return a.m_it == b.m_it;
}

bool operator!=(const RoadSectionCollection::iterator& a, const RoadSectionCollection::iterator& b) {
    return a.m_it != b.m_it;
}

RoadSection& RoadSectionCollection::operator[](const glm::ivec2& position) {
    unsigned int key;
    try {
        key = keyMap.at(position);
    }
    catch (std::out_of_range) {
        key = currentKey++;

        roadSections.emplace(key, RoadSection(position));
    }

    return roadSections[key];
}

RoadSection& RoadSectionCollection::at(const glm::ivec2& position) {
    unsigned int key = keyMap.at(position);
    return roadSections.at(key);
}

const RoadSection& RoadSectionCollection::at(const glm::ivec2& position) const {
    unsigned int key = keyMap.at(position);
    return roadSections.at(key);
}

bool RoadSectionCollection::contains(const glm::ivec2& position) const {
    return keyMap.contains(position);
}

RoadSection& RoadSectionCollection::emplace(RoadSection&& section) {
    unsigned int key;
    try {
        key = keyMap.at(section.position);
    }
    catch (std::out_of_range) {
        key = currentKey++;        
    }

    int sectionLength = section.getLength();
    if ()
}

template RoadSection& RoadSectionCollection::emplace(glm::ivec2&&, RoadSection&&);

RoadSectionCollection::iterator RoadSectionCollection::begin() noexcept {    
    return iterator(roadSections.begin());
}

RoadSectionCollection::iterator RoadSectionCollection::end() noexcept {
    return iterator(roadSections.end());
}

