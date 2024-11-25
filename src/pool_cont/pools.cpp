#include <queue>

#include "pools.hpp"

Pool & Container::AddPool () {
    ContainerElem *element = new ContainerElem (curIdx);
    
    elements.push_back (element);
    curIdx++;

    return element->pool;
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

    ConnectBase (first, second);
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
    ConnectBase (first.idx, second.idx);
}
void Container::ConnectBase (size_t first, size_t second) {
    bool isNeighbor = elements[first]->edges.AddNeighbor (second);
    elements[second]->edges.AddNeighbor (first);

    if (!isNeighbor) {
        marked.push_back(first);
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
    RemoveBase (first.idx, second.idx);
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
    RemoveBase (first, second);
}
void Container::RemoveBase (size_t first, size_t second) {
    MarkAll ();

    if (elements[first]->needRefresh || elements[second]->needRefresh) {
        Refresh (first);
    }

    elements[first]->edges.RemoveNeighbor (second);
    elements[second]->edges.RemoveNeighbor (first);
}

size_t Container::GetVolume (Pool& pool) {
    if (!Contains (pool))  {
        std::cout << "Container doesn't contains pool [" << &pool << "].\n";
        return -1;
    } 
    return GetVolumeBase (pool.idx);
}
size_t Container::GetVolume (size_t pool) {
    if (!Contains (pool))  {
        std::cout << "Container doesn't contains pool [" << pool << "].\n";
        return -1;
    } 
    return GetVolumeBase (pool);
}
size_t Container::GetVolumeBase (size_t pool) {
    MarkAll ();
    if (elements[pool]->needRefresh) {
        Refresh(pool);
    }   
    return elements[pool]->pool.GetVolume ();
}

void Container::AddWater (Pool &pool, size_t size) {
    if (!Contains (pool))  {
        std::cout << "Container doesn't contains pool [" << &pool << "].\n";
        return;
    } 
    AddWaterBase (pool.idx, size);
}
void Container::AddWater (size_t pool, size_t size) {
    if (!Contains (pool))  {
        std::cout << "Container doesn't contains pool [" << pool << "].\n";
        return;
    } 
    AddWaterBase (pool, size);
}
void Container::AddWaterBase (size_t pool, size_t size) {
    elements[pool]->pool.AddWater(size);
    marked.push_back(pool);
}


bool Container::Contains (Pool& pool) {
    size_t poolIdx = pool.idx;
    return  Contains (poolIdx) && &pool == &(elements[poolIdx]->pool);
}
bool Container::Contains (size_t poolIdx) {
    return poolIdx < curIdx;
}

void Container::Refresh (size_t poolIdx) {
    size_t avgVolume = GetConnectionAvgVolume (poolIdx);

    static std::queue<size_t> queue;
    static std::vector<size_t> visited;
    
    queue.push (poolIdx);
    elements[poolIdx]->color = Gray;
    visited.push_back (poolIdx);
    
    while (!queue.empty()) {
        size_t vertex = queue.front();
        queue.pop();
        
        elements[vertex]->pool.volume = avgVolume;

        for (size_t neighbor: elements[vertex]->edges.neighbors) {
            if (elements[neighbor]->color == White) {
                queue.push (neighbor);
                elements[neighbor]->color = Gray;
                visited.push_back (neighbor);
            }
        }
    }

    for (size_t vertex: visited) {
        elements[vertex]->needRefresh = false;
        elements[vertex]->color = White;
    }
    visited.clear();
}

size_t Container::GetConnectionAvgVolume (size_t poolIdx) {
    static std::queue<size_t> queue;
    static std::vector<size_t> visited;
    
    size_t size = 0;
    size_t volumeSum = 0;

    queue.push (poolIdx);
    ++size;
    volumeSum += elements[poolIdx]->pool.volume;
    elements[poolIdx]->color = Gray;
    visited.push_back (poolIdx);
   
    while (!queue.empty()) {
        size_t vertex = queue.front();
        queue.pop();
    
        for (size_t neighbor: elements[vertex]->edges.neighbors) {
            if (elements[neighbor]->color == White) {
                queue.push (neighbor);
                ++size;
                volumeSum += elements[neighbor]->pool.volume;
                elements[neighbor]->color = Gray;
                visited.push_back (neighbor);
            }
        }
        elements[vertex]->needRefresh = true;
    }

    for (size_t vertex: visited) {
        elements[vertex]->color = White;
    }
    visited.clear();
    return volumeSum / size;
}


void Container::MarkAll () {
    static std::queue<size_t> queue;
    static std::vector<size_t> visited;

    for (auto poolIdx: marked) {
        if (elements[poolIdx]->color != White) {
            continue;
        }
        queue.push (poolIdx);
        elements[poolIdx]->color = Gray;
        visited.push_back (poolIdx);
        while (!queue.empty()) {
            size_t vertex = queue.front();
            queue.pop();
            for (size_t neighbor: elements[vertex]->edges.neighbors) {
                if (elements[neighbor]->color == White) {
                    queue.push (neighbor);
                    elements[neighbor]->color = Gray;
                    visited.push_back (neighbor);
                }
            }
            elements[vertex]->needRefresh = true;
        }
    }
    marked.clear();

    for (size_t vertex: visited) {
        elements[vertex]->color = White;
    }
    visited.clear();
}
