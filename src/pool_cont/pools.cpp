#include <queue>

#include "pools.hpp"

Pool & Container::AddPool () {
    Pool *pool = new Pool (curIdx);
    
    pools.push_back (pool);
    edges.push_back (new Edges (curIdx));
    needRefresh.push_back (false);
    colors.push_back (White);
    
    curIdx++;

    return *pool;
}

void Container::Connect (size_t first, size_t second) {
    if (first == second) {
        return;
    }
    if (!Contains (first))  {
        std::cout << "Container doesn't contains pool [" << first << "].\n";
        return;
    } 
    if (!Contains (second))  {
        std::cout << "Container doesn't contains pool [" << second << "].\n";
        return;
    }

    bool isNeighbor = edges[first]->addNeighbor (second);
    edges[second]->addNeighbor (first);

    if (!isNeighbor) {
        marked.push_back(first);
    }
}
void Container::Connect (Pool &first, Pool &second) {
    if (&first == &second) {
        return;
    }
    if (!Contains (first))  {
        std::cout << "Container doesn't contains pool [" << &first << "].\n";
        return;
    } 
    if (!Contains (second))  {
        std::cout << "Container doesn't contains pool [" << &second << "].\n";
        return;
    }
    size_t firstIdx = first.idx;
    size_t secondIdx = second.idx;

    bool isNeighbor = edges[firstIdx]->addNeighbor (secondIdx);
    edges[secondIdx]->addNeighbor (firstIdx);

    if (!isNeighbor) {
        marked.push_back(firstIdx);
    }
}
void Container::RemoveConnection (Pool &first, Pool &second) {
    if (&first == &second) {
        return;
    }
    if (!Contains (first))  {
        std::cout << "Container doesn't contains pool [" << &first << "].\n";
        return;
    } 
    if (!Contains (second))  {
        std::cout << "Container doesn't contains pool [" << &second << "].\n";
        return;
    }

    size_t firstIdx = first.idx;
    size_t secondIdx = second.idx;

    MarkAll ();
    if (needRefresh[firstIdx] || needRefresh[secondIdx]) {
        Refresh (firstIdx);
    }
    
    edges[firstIdx]->removeNeighbor (secondIdx);
    edges[secondIdx]->removeNeighbor (firstIdx);
}

void Container::RemoveConnection (size_t first, size_t second) {
    if (first == second) {
        return;
    }
    if (!Contains (first))  {
        std::cout << "Container doesn't contains pool [" << first << "].\n";
        return;
    } 
    if (!Contains (second))  {
        std::cout << "Container doesn't contains pool [" << second << "].\n";
        return;
    }
    MarkAll ();

    if (needRefresh[first] || needRefresh[second]) {
        Refresh (first);
    }

    edges[first]->removeNeighbor (second);
    edges[second]->removeNeighbor (first);
}

size_t Container::GetVolume (Pool& pool) {
    if (!Contains (pool))  {
        std::cout << "Container doesn't contains pool [" << &pool << "].\n";
        return -1;
    } 
    MarkAll ();

    if (needRefresh.at(pool.idx)) {
        Refresh(pool.idx);
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
    marked.push_back(pool.idx);
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
    size_t poolIdx = pool.idx;
    if (poolIdx < curIdx) {
        return &pool == (pools.at(poolIdx));
    }
    return false;
}
bool Container::Contains (size_t poolIdx) {
    return poolIdx < curIdx;
}


bool Container::checkConnection (size_t firstIdx, size_t secondIdx) {
    if (firstIdx == secondIdx) {
        return true;
    }

    bool ans = false;
    static std::queue<size_t> queue;
    static std::vector<size_t> visited;
    
    queue.push (firstIdx);
    colors[firstIdx] = Gray;
    visited.push_back (firstIdx);
    
    while (!queue.empty() && !ans) {
        size_t vertex = queue.front();
        queue.pop();
        for (size_t neighbor: edges[vertex]->neighbors) {
            if (colors.at(neighbor) == White) {
                if (neighbor == secondIdx) {
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

void Container::Refresh (size_t poolIdx) {
    size_t avgVolume = getConnectionAvgVolume (poolIdx);

    static std::queue<size_t> queue;
    static std::vector<size_t> visited;
    
    queue.push (poolIdx);
    colors[poolIdx] = Gray;
    visited.push_back (poolIdx);
    
    while (!queue.empty()) {
        size_t vertex = queue.front();
        queue.pop();
        
        pools[vertex]->volume = avgVolume;

        for (size_t neighbor: edges[vertex]->neighbors) {
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

size_t Container::getConnectionAvgVolume (size_t poolIdx) {
    static std::queue<size_t> queue;
    static std::vector<size_t> visited;
    
    size_t size = 0;
    size_t volumeSum = 0;

    queue.push (poolIdx);
    ++size;
    volumeSum += pools[poolIdx]->volume;
    colors[poolIdx] = Gray;
    visited.push_back (poolIdx);
   
    while (!queue.empty()) {
        size_t vertex = queue.front();
        queue.pop();
    
        for (size_t neighbor: edges[vertex]->neighbors) {
            if (colors.at(neighbor) == White) {
                queue.push (neighbor);
                ++size;
                volumeSum += pools[neighbor]->volume;
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

    for (auto poolIdx: marked) {
        if (colors[poolIdx] != White) {
            continue;
        }
        queue.push (poolIdx);
        colors[poolIdx] = Gray;
        visited.push_back (poolIdx);
        while (!queue.empty()) {
            size_t vertex = queue.front();
            queue.pop();
            for (size_t neighbor: edges[vertex]->neighbors) {
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
