#include <queue>

#include "pools.hpp"

void Container::MarkNeedRefresh (size_t pool_idx) {
    static std::queue<size_t> queue;
    static std::vector<size_t> visited;
    // visited.reserve (1000000);

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

    for (size_t vertex: visited) {
        colors[vertex] = White;
    }
    visited.clear();
}

bool Container::checkConnection (size_t a_idx, size_t b_idx) {
    if (a_idx == b_idx) {
        return true;
    }

    bool ans = false;
    static std::queue<size_t> queue;
    static std::vector<size_t> visited;
    // visited.reserve (1000000);
    
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
    // visited.reserve (1000000);
    
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
    // visited.reserve (1000000);
    
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
    // visited.reserve (1000000);

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
