#include <queue>

#include "pools.hpp"

Pool & Container::AddPool () {
    Pool *pool = new Pool (cur_idx);
    pools.push_back (pool);
    edges.push_back (new Edges (cur_idx));
    needRefresh.push_back (false);
    colors.push_back (White);
    cur_idx++;
    return *pool;
}

void Container::Connect (size_t a, size_t b) {
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

    bool isNeighbor = edges[a]->addNeighbor (b);
    edges[b]->addNeighbor (a);

    if (!isNeighbor) {
        marked.push_back(a);
    }
}
void Container::Connect (Pool &a, Pool &b) {
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

    bool isNeighbor = edges[a_idx]->addNeighbor (b_idx);
    edges[b_idx]->addNeighbor (a_idx);

    if (!isNeighbor) {
        marked.push_back(a_idx);
    }
}
void Container::RemoveConnection (Pool &a, Pool &b) {
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

    MarkAll ();
    if (needRefresh[a_idx] || needRefresh[b_idx]) {
        Refresh (a_idx);
    }
    
    edges[a_idx]->removeNeighbor (b_idx);
    edges[b_idx]->removeNeighbor (a_idx);
}

void Container::RemoveConnection (size_t a, size_t b) {
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

    if (needRefresh[a] || needRefresh[b]) {
        Refresh (a);
    }

    edges[a]->removeNeighbor (b);
    edges[b]->removeNeighbor (a);
}

size_t Container::GetVolume (Pool& pool) {
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
size_t Container::GetVolume (size_t pool) {
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

void Container::AddWater (Pool &pool, size_t size) {
    if (!Contains (pool))  {
        std::cout << "Container doesn't contains pool [" << &pool << "].\n";
        return;
    } 
    pool.addWater(size);
    marked.push_back(pool.idx_);
}
void Container::AddWater (size_t pool, size_t size) {
    if (!Contains (pool))  {
        std::cout << "Container doesn't contains pool [" << pool << "].\n";
        return;
    } 
    pools[pool]->addWater(size);
    marked.push_back(pool);
}
bool Container::Contains (Pool& pool) {
    size_t pool_idx = pool.idx_;
    if (pool_idx < cur_idx) {
        return &pool == (pools.at(pool_idx));
    }
    return false;
}
bool Container::Contains (size_t pool_idx) {
    return pool_idx < cur_idx;
}


bool Container::checkConnection (size_t a_idx, size_t b_idx) {
    if (a_idx == b_idx) {
        return true;
    }

    bool ans = false;
    static std::queue<size_t> queue;
    static std::vector<size_t> visited;
    
    queue.push (a_idx);
    colors[a_idx] = Gray;
    visited.push_back (a_idx);
    
    while (!queue.empty() && !ans) {
        size_t vertex = queue.front();
        queue.pop();
        for (size_t neighbor: edges[vertex]->neighbors_) {
            if (colors.at(neighbor) == White) {
                if (neighbor == b_idx) {
                    ans = true;
                    break;
                }
                queue.push (neighbor);
                colors[neighbor] = Gray;
                visited.push_back (neighbor);
            }
        }
    }
    while (!queue.empty()) {
        queue.pop();
    }

    for (size_t vertex: visited) {
        colors[vertex] = White;
    }
    visited.clear();
    return ans;
}

void Container::Refresh (size_t pool_idx) {
    size_t avgVolume = getConnectionAvgVolume (pool_idx);

    static std::queue<size_t> queue;
    static std::vector<size_t> visited;
    
    queue.push (pool_idx);
    colors[pool_idx] = Gray;
    visited.push_back (pool_idx);
    
    while (!queue.empty()) {
        size_t vertex = queue.front();
        queue.pop();
        
        pools[vertex]->volume_ = avgVolume;

        for (size_t neighbor: edges[vertex]->neighbors_) {
            if (colors.at(neighbor) == White) {
                queue.push (neighbor);
                colors[neighbor] = Gray;
                visited.push_back (neighbor);
            }
        }
    }

    for (size_t vertex: visited) {
        needRefresh[vertex] = false;
        colors[vertex] = White;
    }
    visited.clear();
}

size_t Container::getConnectionAvgVolume (size_t pool_idx) {
    static std::queue<size_t> queue;
    static std::vector<size_t> visited;
    
    size_t size = 0;
    size_t volumeSum = 0;

    queue.push (pool_idx);
    ++size;
    volumeSum += pools[pool_idx]->volume_;
    colors[pool_idx] = Gray;
    visited.push_back (pool_idx);
   
    while (!queue.empty()) {
        size_t vertex = queue.front();
        queue.pop();
    
        for (size_t neighbor: edges[vertex]->neighbors_) {
            if (colors.at(neighbor) == White) {
                queue.push (neighbor);
                ++size;
                volumeSum += pools[neighbor]->volume_;
                colors[neighbor] = Gray;
                visited.push_back (neighbor);
            }
        }
        needRefresh[vertex] = true;
    }

    for (size_t vertex: visited) {
        colors[vertex] = White;
    }
    visited.clear();
    return volumeSum / size;
}


void Container::MarkAll () {
    static std::queue<size_t> queue;
    static std::vector<size_t> visited;

    for (auto pool_idx: marked) {
        if (colors[pool_idx] != White) {
            continue;
        }
        queue.push (pool_idx);
        colors[pool_idx] = Gray;
        visited.push_back (pool_idx);
        while (!queue.empty()) {
            size_t vertex = queue.front();
            queue.pop();
            for (size_t neighbor: edges[vertex]->neighbors_) {
                if (colors.at(neighbor) == White) {
                    queue.push (neighbor);
                    colors[neighbor] = Gray;
                    visited.push_back (neighbor);
                }
            }
            needRefresh[vertex] = true;
        }
    }
    marked.clear();

    for (size_t vertex: visited) {
        colors[vertex] = White;
    }
    visited.clear();
}
