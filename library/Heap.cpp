#include "Heap.h"

void Heap::Add(NodePtr bor_node) {
    data.push_back(bor_node);
    size_t pos = data.size() - 1;
    while (pos) {
        size_t parent = (pos - 1) / 2;
        if (data[parent]->prior > data[pos]->prior) {
            std::swap(data[parent], data[pos]);
            pos = parent;
        } else {
            break;
        }
    }
}

void Heap::Repair(size_t pos) {
    if (pos * 2 + 1 >= data.size()) {
        return;
    }
    if (pos * 2 + 1 == data.size() - 1 || IsPriorLess(data[pos * 2 + 1], data[pos * 2 + 2])) {
        if (IsPriorLess(data[pos * 2 + 1], data[pos])) {
            std::swap(data[pos], data[pos * 2 + 1]);
            Repair(pos * 2 + 1);
        }
    } else {
        if (IsPriorLess(data[pos * 2 + 2], data[pos])) {
            std::swap(data[pos], data[pos * 2 + 2]);
            Repair(pos * 2 + 2);
        }
    }
}

size_t Heap::Size() {
    return data.size();
}

NodePtr Heap::GetMin() {
    if (data.empty()) {
        return nullptr;
    }
    std::swap(data[0], data.back());
    NodePtr min = data.back();
    data.pop_back();
    Repair(0);
    return min;
}