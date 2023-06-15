#include "Converter.h"

uint8_t ConvertVToInt8(std::vector<bool> bits) {
    uint8_t byte = 0;
    for (auto bit: bits) {
        byte <<= 1;
        byte |= bit;
    }

    return byte;
}

uint16_t ConvertVToInt16(std::vector<bool> bits) {
    uint16_t byte = 0;
    for (auto bit: bits) {
        byte <<= 1;
        byte |= bit;
    }

    return byte;
}

void IncreaseVector(std::vector<bool> &bits) {
    size_t pos = bits.size() - 1;
    while (bits[pos]) {
        bits[pos] = 0;
        if (!pos) {
            return;
        }
        --pos;
    }
    bits[pos] = 1;
}