#pragma once

#include <cstdint>
#include <iostream>
#include <memory>

class BorNode;

using NodePtr = std::shared_ptr<BorNode>;

class BorNode {
public:
    size_t prior = 0;
    uint16_t symbol = 0;
    NodePtr left = nullptr;
    NodePtr right = nullptr;

    BorNode(size_t prior, uint16_t symbol);

    BorNode(NodePtr left, NodePtr right);
};

NodePtr AddNode(size_t prior, uint16_t symbol = 0);

NodePtr AddNode(NodePtr left = nullptr, NodePtr right = nullptr);

size_t GetPrior(NodePtr node);

bool IsPriorLess(NodePtr node1, NodePtr node2);