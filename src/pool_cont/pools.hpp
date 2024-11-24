#pragma once
#include <iostream>
#include <vector>

enum Colors {
    White = 0,
    Gray = 1, 
    Black = 2
};

class Pool {
    friend class Container;
    size_t volume_ = 0;
    size_t idx_;

    Pool (size_t idx) : idx_(idx) {;}

    size_t getVolume () {
        return volume_;
    }
    void addWater (size_t size) {
        volume_ += size;
    }
};


class Edges {
    friend class Container;
    size_t vertex_;
    std::vector<size_t> neighbors_;
public:
    Edges (size_t vertex) : vertex_(vertex){};
    bool addNeighbor (size_t neighbor) {
        for (auto it = neighbors_.begin (); it != neighbors_.end(); ++it) {
            if (*it == neighbor) {
                return false;
            }
        }
        neighbors_.push_back (neighbor);
        return true;
    }
    bool removeNeighbor (size_t neighbor) {
        for (auto it = neighbors_.begin (); it != neighbors_.end(); ++it) {
            if (*it == neighbor) {
                neighbors_.erase(it);
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

    size_t cur_idx = 0;
    size_t max_capacity = 0;
public:
    Container () {}
    
    Container (const size_t size) :max_capacity(size){
        pools.reserve (size);
        edges.reserve (size);
        needRefresh.reserve (size);
        colors.reserve (size);
    }
    
    size_t getSize () {
        return pools.size();
    }

    Pool &AddPool ();

    void Connect (size_t a, size_t b);
    void Connect (Pool &a, Pool &b);
    
    void RemoveConnection (Pool &a, Pool &b);
    void RemoveConnection (size_t a, size_t b);
    
    size_t GetVolume (Pool& pool);
    size_t GetVolume (size_t pool);
    
    void AddWater (Pool &pool, size_t size);
    void AddWater (size_t pool, size_t size);
    
    bool Contains (Pool& pool);
    bool Contains (size_t pool_idx);

    ~Container () {
        for (auto pool_edges: edges) {
            delete pool_edges;
        }
        for (auto pool:pools) {
            delete pool;
        }
    }

private:
    void MarkAll ();
    void Refresh (size_t pool_idx);
    size_t getConnectionAvgVolume (size_t pool_idx);
    bool checkConnection (size_t a_idx, size_t b_idx);
};

