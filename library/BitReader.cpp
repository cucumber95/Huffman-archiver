#include "BitReader.h"

BitReader::BitReader(std::istream &input_stream)
        : fin_(input_stream) {
}

std::vector<bool> BitReader::ReadBits(size_t count) {
    std::vector<bool> bits;
    if (count > buffer_size_) {
        UpdateBuffer();
    }

    if (count > buffer_size_) {
        throw std::exception();
    }

    for (size_t i = 0; i < count; ++i) {
        bits.push_back(buffer_ & (1 << (buffer_size_ - 1)));
        --buffer_size_;
    }

    return bits;
}

void BitReader::Reset() {
    fin_.clear();
    fin_.seekg(0);
    buffer_ = 0;
    buffer_size_ = 0;
}

void BitReader::UpdateBuffer() {
    while (buffer_size_ <= 8) {
        uint8_t new_byte;
        fin_.read(reinterpret_cast<char *>(&new_byte), 1);
        if (fin_.gcount() == 0) {
            return;
        }

        buffer_size_ += 8;
        buffer_ <<= 8;
        buffer_ |= new_byte;
    }
}