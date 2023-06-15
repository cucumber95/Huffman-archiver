#include "Bor.h"

size_t GetPrior(NodePtr node) {
    if (node == nullptr) {
        return 0;
    }
    return node->prior;
}

bool IsPriorLess(NodePtr node1, NodePtr node2) {
    if (!GetPrior(node2)) {
        return true;
    }
    if (!GetPrior(node1)) {
        return false;
    }
    return GetPrior(node1) < GetPrior(node2);
}

BorNode::BorNode(size_t prior, uint16_t symbol) {
    this->prior = prior;
    this->symbol = symbol;
}

BorNode::BorNode(NodePtr left, NodePtr right) {
    this->left = left;
    this->right = right;
    this->prior = GetPrior(left) + GetPrior(right);
}

NodePtr AddNode(size_t prior, uint16_t symbol) {
    return std::make_shared<BorNode>(prior, symbol);
}

NodePtr AddNode(NodePtr left, NodePtr right) {
    return std::make_shared<BorNode>(left, right);
}
