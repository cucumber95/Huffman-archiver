#pragma once

#include "Bor.h"

#include <cstdint>
#include <vector>

class Heap {
public:
    void Add(NodePtr bor_node);

    size_t Size();

    NodePtr GetMin();

private:
    std::vector<NodePtr> data;

    void Repair(size_t pos);
};