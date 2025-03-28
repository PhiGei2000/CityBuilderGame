#pragma once
#include <array>
#include <glm/glm.hpp>
#include <iterator>
#include <utility>
#include <variant>

template<typename TValue>
struct Quadtree;

template<typename TValue>
struct Quadtree {
    struct NodeType {
        glm::ivec2 key;
        int nodeSize;
        bool isLeaf;

        TValue value;
        Quadtree<TValue>* subtrees;
    } data;

    inline const NodeType& operator[](const glm::ivec2& key) const {
        return getNode(key);
    }

    inline Quadtree()
        : data(glm::ivec2(0), 0, false, TValue(), nullptr) {
    }

    inline Quadtree(const glm::ivec2& key, int size, const TValue& value)
        : data(key, size, true, value, nullptr) {
    }

    inline Quadtree(const Quadtree<TValue>& other) {
        if (other.data.isLeaf) {
            data = other.data;
        }
        else {
            data = NodeType{other.data.key, other.data.nodeSize, false, TValue(), new Quadtree<TValue>[4]};
            for (int i = 0; i < 4; i++) {
                data.subtrees[i] = other.data.subtrees[i];
            }
        }
    }

    inline Quadtree(Quadtree<TValue>&& other) {
        data = other.data;
        other.data.subtrees = nullptr;
    }

    inline ~Quadtree() {
        if (data.subtrees != nullptr) {
            for (int i = 0; i < 4; i++) {
                data.subtrees[i].~Quadtree();
            }

            delete[] data.subtrees;
            data.subtrees = nullptr;
        }
    }

    inline const NodeType& getNode(const glm::ivec2& key) const {
        if (data.isLeaf) {
            return data;
        }

        int subnodeIndex = getSubnodeIndex(key);
        return data.subtrees[subnodeIndex].getNode(key);
    }

    inline NodeType& getNode(const glm::ivec2& key) {
        if (data.isLeaf) {
            return data;
        }

        int subnodeIndex = getSubnodeIndex(key);
        return data.subtrees[subnodeIndex].getNode(key);
    }

    inline NodeType& createLeaf(const glm::ivec2& key, int size = 1) {
        if (data.nodeSize <= size) {
            return data;
        }

        int subtreeIndex = getSubnodeIndex(key);
        split();

        return data.subtrees[subtreeIndex].createLeaf(key, size);
    }

    inline NodeType& setValue(const glm::ivec2& key, const TValue& value) {
        if (data.nodeSize == 1 && data.key == key) {
            data.value = value;
            return data;
        }

        int subtreeIndex = getSubnodeIndex(key);
        // if (subtreeIndex == -1) {
        //     return;
        // }

        split();

        return data.subtrees[subtreeIndex].setValue(key, value);
    }

    void split() {
        if (!data.isLeaf || data.nodeSize == 1) {
            return;
        }

        int halfNodeSize = data.nodeSize / 2;

        data.subtrees = new Quadtree<TValue>[4];
        data.subtrees[0] = Quadtree<TValue>(data.key, halfNodeSize, data.value);
        data.subtrees[1] = Quadtree<TValue>(data.key + halfNodeSize * glm::ivec2(1, 0), halfNodeSize, data.value);
        data.subtrees[2] = Quadtree<TValue>(data.key + halfNodeSize * glm::ivec2(0, 1), halfNodeSize, data.value);
        data.subtrees[3] = Quadtree<TValue>(data.key + halfNodeSize * glm::ivec2(1, 1), halfNodeSize, data.value);
        data.isLeaf = false;
    }

    void reduce() {
        if (data.isLeaf) {
            return;
        }

        for (int i = 0; i < 4; i++) {
            data.subtrees[i].reduce();
        }

        if (!data.subtrees[0].data.isLeaf) {
            return;
        }

        for (int i = 1; i < 4; i++) {
            if (!data.subtrees[i].data.isLeaf) {
                return;
            }

            if (data.subtrees[i].data.value != data.subtrees[0].data.value) {
                return;
            }
        }

        data.value = data.subtrees[0].data.value;
        delete[] data.subtrees;
        data.subtrees = nullptr;
        data.isLeaf = true;
    }

    template<int size>
    inline static Quadtree<TValue> fromValues(TValue** values) {
        Quadtree result = getQuadtree<size>(values);

        result.reduce();
        return result;
    }

    operator NodeType() const {
        return data;
    }

    inline std::vector<NodeType> getLeafs() const {
        std::vector<NodeType> result;
        getLeafNodes(result);

        return result;
    }

    inline Quadtree<TValue>& operator=(const Quadtree<TValue>& other) {
        if (other.data.isLeaf) {
            data = other.data;
        }
        else {
            data = NodeType{other.data.key, other.data.nodeSize, false, TValue(), new Quadtree<TValue>[4]};
            for (int i = 0; i < 4; i++) {
                data.subtrees[i] = other.data.subtrees[i];
            }
        }

        return *this;
    }

    inline Quadtree<TValue>& operator=(Quadtree<TValue>&& other) {
        data = other.data;
        other.data.subtrees = nullptr;

        return *this;
    }

  private:
    inline void getLeafNodes(std::vector<NodeType>& nodes) const {
        if (data.isLeaf) {
            nodes.emplace_back(data);
            return;
        }

        for (int i = 0; i < 4; i++) {
            data.subtrees[i].getLeafNodes(nodes);
        }
    }

    template<int size>
    inline static Quadtree<TValue> getQuadtree(TValue** values, const glm::ivec2& offset = glm::ivec2(0)) {
        constexpr int halfSize = size / 2;

        Quadtree<TValue>* subtrees = new Quadtree<TValue>[4];
        subtrees[0] = getQuadtree<halfSize>(values, offset);
        subtrees[1] = getQuadtree<halfSize>(values, offset + glm::ivec2(halfSize, 0));
        subtrees[2] = getQuadtree<halfSize>(values, offset + glm::ivec2(0, halfSize));
        subtrees[3] = getQuadtree<halfSize>(values, offset + glm::ivec2(halfSize, halfSize));

        return Quadtree<TValue>{offset, size, false, nullptr, subtrees};
    }

    template<>
    inline static Quadtree<TValue> getQuadtree<1>(TValue** values, const glm::ivec2& offset) {
        return Quadtree<TValue>(offset, 1, values[offset.x][offset.y]);
    }

    int getSubnodeIndex(const glm::ivec2& key) const {
        if (key.x < data.key.x || key.x > data.key.x + data.nodeSize || key.y < data.key.y || key.y > data.key.y + data.nodeSize) {
            return -1;
        }

        const int halfNodeSize = data.nodeSize / 2;
        // if (key.x < data.key.x + halfNodeSize && key.y < data.key.y + halfNodeSize) {
        //     return 0;
        // }

        // if (key.x >= data.key.x + halfNodeSize && key.y < data.key.y + halfNodeSize) {
        //     return 1;
        // }

        // if (key.x < data.key.x + halfNodeSize && key.y >= data.key.y + halfNodeSize) {
        //     return 2;
        // }

        // if (key.x >= data.key.x + halfNodeSize && key.y >= data.key.y + halfNodeSize) {
        //     return 3;
        // }

        int index = 0;
        if (key.x >= data.key.x + halfNodeSize) {
            index += 1;
        }

        if (key.y >= data.key.y + halfNodeSize) {
            index += 2;
        }

        return index;
    }
};
