#pragma once

#include "BitReader.h"
#include "Bor.h"
#include "Heap.h"

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>

class BitReader {
public:
    BitReader(std::istream &input_stream);

    std::vector<bool> ReadBits(size_t count);

    void Reset();

private:
    std::istream &fin_;
    uint16_t buffer_ = 0;
    uint8_t buffer_size_ = 0;

    void UpdateBuffer();
};
