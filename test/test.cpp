#include "BitReader.h"
#include "Converter.h"
#include "BitWriter.h"
#include "Bor.h"
#include "Heap.h"

#define CATCH_CONFIG_MAIN

#include "catch.hpp"

void CheckRead(const std::vector<bool>& bits, const std::vector<uint8_t>& data, size_t& bit_ind, size_t& byte_ind) {
    for (size_t i = 0; i < bits.size(); ++i) {
        REQUIRE(byte_ind < data.size());
        uint8_t byte = data[byte_ind];
        bool bit = byte & (1 << (7 - bit_ind));
        REQUIRE(bit == bits[i]);
        ++bit_ind;
        if (bit_ind == 8) {
            bit_ind = 0;
            ++byte_ind;
        }
    }
}

TEST_CASE("TestBitReader") {
    std::vector<uint8_t> data = {15, 240, 15};
    std::stringstream stream;
    for (size_t i = 0; i < data.size(); ++i) {
        stream.write(reinterpret_cast<char*>(&data[i]), 1);
    }

    size_t bit_ind = 0;
    size_t byte_ind = 0;
    BitReader reader(stream);
    std::vector<bool> bits = reader.ReadBits(9);
    REQUIRE(bits.size() == 9);
    CheckRead(bits, data, bit_ind, byte_ind);
    bits = reader.ReadBits(8);
    REQUIRE(bits.size() == 8);
    CheckRead(bits, data, bit_ind, byte_ind);
    bits = reader.ReadBits(7);
    REQUIRE(bits.size() == 7);
    CheckRead(bits, data, bit_ind, byte_ind);

    reader.Reset();
    bit_ind = 0;
    byte_ind = 0;
    bits = reader.ReadBits(15);
    REQUIRE(bits.size() == 15);
    CheckRead(bits, data, bit_ind, byte_ind);
    bits = reader.ReadBits(1);
    REQUIRE(bits.size() == 1);
    CheckRead(bits, data, bit_ind, byte_ind);
    bits = reader.ReadBits(1);
    REQUIRE(bits.size() == 1);
    CheckRead(bits, data, bit_ind, byte_ind);

    reader.Reset();

}

TEST_CASE("TestConverter") {
    std::vector<bool> bits1 = {1, 0, 1, 0};
    std::vector<bool> bits2 = {1, 0, 1, 0, 0, 1, 1, 1};
    std::vector<bool> bits1_1 = {0, 1, 0, 1, 0};
    uint8_t ans1 = 10;
    uint8_t ans2 = 167;
    REQUIRE(ConvertVToInt8(bits1) == ans1);
    REQUIRE(ConvertVToInt8(bits1_1) == ans1);
    REQUIRE(ConvertVToInt8(bits2) == ans2);

    REQUIRE(ConvertIntToV(ans1, 4) == bits1);
    REQUIRE(ConvertIntToV(ans1, 5) == bits1_1);
    REQUIRE(ConvertIntToV(ans2, 8) == bits2);

    IncreaseVector(bits1);
    REQUIRE(ConvertVToInt8(bits1) == ans1 + 1);
    IncreaseVector(bits1);
    REQUIRE(ConvertVToInt8(bits1) == ans1 + 2);
}

TEST_CASE("TestBitWriter") {
    std::stringstream stream;
    BitWriter writer(stream);
    std::vector<bool> bits = {1, 0, 0, 1, 1, 0, 1, 1, 0};
    writer.WriteBits(bits);
    uint8_t byte;
    stream.read(reinterpret_cast<char*>(&byte), 1);
    REQUIRE(byte == 155);
    bits = {0, 0, 0, 1, 1, 0, 1, 0, 1, 1};
    writer.WriteBits(bits);
    stream.read(reinterpret_cast<char*>(&byte), 1);
    REQUIRE(byte == 13);
    writer.Push();
    stream.read(reinterpret_cast<char*>(&byte), 1);
    REQUIRE(byte == 96);
}

TEST_CASE("TestBorAndHeap") {
//    Bor:
//    Node[i]->left = Node[i * 2 + 1]
//    Node[i]->right = Node[i * 2 + 2];
    NodePtr node7 = AddNode(2, 'a');
    NodePtr node8 = AddNode(3, 'b');
    NodePtr node13 = AddNode(6, 'c');
    NodePtr node3 = AddNode(node7, node8);
    NodePtr node4 = AddNode(nullptr, nullptr);
    NodePtr node6 = AddNode(node13, nullptr);
    NodePtr node1 = AddNode(node3, node4);
    NodePtr node2 = AddNode(nullptr, node6);
    NodePtr node0 = AddNode(node1, node2);
    REQUIRE(GetPrior(node0) == 11);
    REQUIRE(GetPrior(node2) == 6);
    REQUIRE(GetPrior(node7) == 2);
    REQUIRE(IsPriorLess(node7, node8) == true);
    REQUIRE(IsPriorLess(node0, node1) == false);
    REQUIRE(node0->left == node1);
    REQUIRE(node2->left == nullptr);
    NodePtr zero_node = AddNode();
    REQUIRE(zero_node->left == nullptr);
    REQUIRE(zero_node->right == nullptr);

//    Heap:
    node0 = AddNode(0);
    node1 = AddNode(1);
    node2 = AddNode(2);
    NodePtr node2_2 = AddNode(2);
    node3 = AddNode(3);

    Heap heap;
    heap.Add(node0);
    heap.Add(node1);
    heap.Add(node2);
    heap.Add(node2_2);
    heap.Add(node3);
    NodePtr min;
    min = heap.GetMin();
    REQUIRE(min->prior == 0);
    min = heap.GetMin();
    REQUIRE(min->prior == 1);
    min = heap.GetMin();
    REQUIRE(min->prior == 2);
    min = heap.GetMin();
    REQUIRE(min->prior == 2);
    heap.Add(node0);
    min = heap.GetMin();
    REQUIRE(min->prior == 0);
    min = heap.GetMin();
    REQUIRE(min->prior == 3);
    min = heap.GetMin();
    REQUIRE(min == nullptr);
}