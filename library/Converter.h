#pragma once

#include <vector>
#include <cstdint>

uint8_t ConvertVToInt8(std::vector<bool> bits);

uint16_t ConvertVToInt16(std::vector<bool> bits);

template<typename T>
std::vector<bool> ConvertIntToV(T number, size_t size) {
    std::vector<bool> bits;
    for (size_t i = 0; i < size; ++i) {
        bits.push_back(number & (1 << (size - i - 1)));
    }

    return bits;
}

void IncreaseVector(std::vector<bool> &bits);

