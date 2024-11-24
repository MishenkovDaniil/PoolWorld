#pragma once
#include <iostream>
#include <vector>

class Pool {
public: 
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
    size_t vertex_;
public:
    std::vector<size_t> neighbors_;
    // std::vector<bool> is_neighbor_;
    Edges (size_t vertex, size_t max = 0) : vertex_(vertex), neighbors_(std::vector<size_t> ()) {};
    void addNeighbor (size_t neighbor) {
        // if (contains(neighbor)) {
        //     return;
        // }
        // if (neighbor >= is_neighbor_.size ()) {
            // is_neighbor_.resize (neighbor + 1);
        // }
        // is_neighbor_[neighbor] = true;
        for (auto it = neighbors_.begin (); it != neighbors_.end(); ++it) {
            if (*it == neighbor) {
                return;
            }
        }
        neighbors_.push_back (neighbor);
    }
    void removeNeighbor (size_t neighbor) {
        // if (!contains(neighbor)){
        //     // std::cout << "Error: cannot remove neighbor from vertex " << vertex_ << ".\n";
        //     return;
        // }
        // is_neighbor_[neighbor] = false;
        for (auto it = neighbors_.begin (); it != neighbors_.end(); ++it) {
            if (*it == neighbor) {
                neighbors_.erase(it);
                break;
            }
        }
    }
    // bool contains (size_t neighbor) {
    //     if (neighbor >= is_neighbor_.size ()) {
    //         return false;
    //     } 
    //     return is_neighbor_[neighbor];
    // }
};

enum Colors {
    White = 0,
    Gray = 1, 
    Black = 2
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
    Pool &AddPool () {
        Pool *pool = new Pool (cur_idx);
        pools.push_back (pool);
        edges.push_back (new Edges (cur_idx, max_capacity));
        needRefresh.push_back (false);
        colors.push_back (White);
        cur_idx++;
        return *pool;
    }

    void Connect (size_t a, size_t b) {
        if (a == b) {
            return;
        }
        if (!Contains (a))  {
            std::cout << "Container doesn't contains pool [" << a << "].\n";
            return;
        } 
        if (!Contains (b))  {
            std::cout << "Container doesn't contains pool [" << b << "].\n";
            return;
        }

        // bool isConnected = checkConnection(a, b);

        edges[a]->addNeighbor (b);
        edges[b]->addNeighbor (a);

        // if (!isConnected) {
            marked.push_back(a);
        //    MarkNeedRefresh (a);
        // }
    }
    void Connect (Pool &a, Pool &b) {
        if (&a == &b) {
            return;
        }
        if (!Contains (a))  {
            std::cout << "Container doesn't contains pool [" << &a << "].\n";
            return;
        } 
        if (!Contains (b))  {
            std::cout << "Container doesn't contains pool [" << &b << "].\n";
            return;
        }
        size_t a_idx = a.idx_;
        size_t b_idx = b.idx_;

        // if (edges[a_idx]->contains(b_idx)) {
        //     return;
        // }

        // bool isConnected = checkConnection(a_idx, b_idx);

        edges[a_idx]->addNeighbor (b_idx);
        edges[b_idx]->addNeighbor (a_idx);


        // if (!isConnected) {
            marked.push_back(a_idx);
            // MarkNeedRefresh (a_idx);
        // }
    }
    void RemoveConnection (Pool &a, Pool &b) {
        if (&a == &b) {
            return;
        }
        if (!Contains (a))  {
            std::cout << "Container doesn't contains pool [" << &a << "].\n";
            return;
        } 
        if (!Contains (b))  {
            std::cout << "Container doesn't contains pool [" << &b << "].\n";
            return;
        }

        size_t a_idx = a.idx_;
        size_t b_idx = b.idx_;

        if(!checkConnection(a_idx, b_idx)) {
            // std::cout << "Error: cannot remove connection between [" << &a << "] and [" << &b << "] because they were not connected.\n";
            return;
        } 
        MarkAll ();
        if (needRefresh[a_idx] || needRefresh[b_idx]) {
            Refresh (a_idx);
        }
        
        edges[a_idx]->removeNeighbor (b_idx);
        edges[b_idx]->removeNeighbor (a_idx);
    }

    void RemoveConnection (size_t a, size_t b) {
        if (a == b) {
            return;
        }
        if (!Contains (a))  {
            std::cout << "Container doesn't contains pool [" << a << "].\n";
            return;
        } 
        if (!Contains (b))  {
            std::cout << "Container doesn't contains pool [" << b << "].\n";
            return;
        }
        MarkAll ();

        // if(checkConnection(a, b)) {
            // std::cout << "Error: cannot remove connection between [" << a << "] and [" << b << "] because they were not connected.\n";
            // return;
        if (needRefresh[a] || needRefresh[b]) {
            Refresh (a);
        }
        // } 

        
        edges[a]->removeNeighbor (b);
        edges[b]->removeNeighbor (a);
    }

    size_t GetVolume (Pool& pool) {
        if (!Contains (pool))  {
            std::cout << "Container doesn't contains pool [" << &pool << "].\n";
            return -1;
        } 
        MarkAll ();

        if (needRefresh.at(pool.idx_)) {
            Refresh(pool.idx_);
        }   
        return pool.getVolume ();
    }
    size_t GetVolume (size_t pool) {
        if (!Contains (pool))  {
            std::cout << "Container doesn't contains pool [" << pool << "].\n";
            return -1;
        } 
        MarkAll ();
        if (needRefresh.at(pool)) {
            Refresh(pool);
        }   
        return pools[pool]->getVolume ();
    }

    void AddWater (Pool &pool, size_t size) {
        if (!Contains (pool))  {
            std::cout << "Container doesn't contains pool [" << &pool << "].\n";
            return;
        } 
        pool.addWater(size);
        // MarkNeedRefresh (pool.idx_);
        marked.push_back(pool.idx_);
    }
    void AddWater (size_t pool, size_t size) {
        if (!Contains (pool))  {
            std::cout << "Container doesn't contains pool [" << pool << "].\n";
            return;
        } 
        pools[pool]->addWater(size);
        // MarkNeedRefresh (pool);
        marked.push_back(pool);
    }
    bool Contains (Pool& pool) {
        size_t pool_idx = pool.idx_;
        if (pool_idx < cur_idx) {
            return &pool == (pools.at(pool_idx));
        }
        return false;
    }
    bool Contains (size_t pool_idx) {
        return pool_idx < cur_idx;
    }

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
    void MarkNeedRefresh (size_t pool_idx);
    size_t getConnectionAvgVolume (size_t pool_idx);
    bool checkConnection (size_t a_idx, size_t b_idx);
};

