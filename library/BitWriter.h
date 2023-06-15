#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>

class BitWriter {
public:
    BitWriter(std::ostream &output_stream);

    void WriteBits(std::vector<bool> bits);

    void Push();

private:
    std::ostream &fout_;
    uint8_t buffer_size_ = 0;
    uint8_t buffer_ = 0;
};