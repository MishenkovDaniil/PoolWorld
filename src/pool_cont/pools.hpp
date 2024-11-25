#pragma once
#include <iostream>
#include <vector>

enum Colors {
    White = 0,
    Gray = 1
};

class Pool {
    friend class Container;
    size_t volume = 0;
    size_t idx;

    Pool (size_t poolIdx) : idx(poolIdx) {}

    size_t GetVolume () {
        return volume;
    }
    void AddWater (size_t size) {
        volume += size;
    }
};


class Edges {
    friend class Container;
    size_t vertex;
    std::vector<size_t> neighbors;
public:
    Edges (size_t mainVertex) : vertex(mainVertex){};
    bool AddNeighbor (size_t neighbor) {
        for (auto it = neighbors.begin (); it != neighbors.end(); ++it) {
            if (*it == neighbor) {
                return false;
            }
        }
        neighbors.push_back (neighbor);
        return true;
    }
    bool RemoveNeighbor (size_t neighbor) {
        for (auto it = neighbors.begin (); it != neighbors.end(); ++it) {
            if (*it == neighbor) {
                neighbors.erase(it);
                return true;
            }
        }
        return false;
    }
};

class Container {
    std::vector<Pool *> pools = std::vector<Pool *>();
    std::vector<Edges*> edges = std::vector<Edges*>();
    std::vector<bool> needRefresh = std::vector<bool>();
    std::vector<Colors> colors = std::vector<Colors>();
    std::vector<size_t> marked;

    size_t curIdx = 0;
public:
    Container () {}
    Container (const size_t size) {
        pools.reserve (size);
        edges.reserve (size);
        needRefresh.reserve (size);
        colors.reserve (size);
    }
    
    size_t GetSize () {
        return pools.size();
    }

    Pool &AddPool ();

    void Connect (size_t a, size_t b);
    void Connect (Pool &a, Pool &b);
    void ConnectBase (size_t first, size_t second);
     
    void RemoveConnection (Pool &a, Pool &b);
    void RemoveConnection (size_t a, size_t b);
    
    size_t GetVolume (Pool& pool);
    size_t GetVolume (size_t pool);
    
    void AddWater (Pool &pool, size_t size);
    void AddWater (size_t pool, size_t size);
    
    bool Contains (Pool& pool);
    bool Contains (size_t poolIdx);

    ~Container () {
        for (auto poolEdges: edges) {
            delete poolEdges;
        }
        for (auto pool:pools) {
            delete pool;
        }
    }

private:
    void MarkAll ();
    void Refresh (size_t poolIdx);
    size_t GetConnectionAvgVolume (size_t poolIdx);
};

