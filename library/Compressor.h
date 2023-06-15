#pragma once

#include "BitReader.h"
#include "BitWriter.h"
#include "Bor.h"
#include "Heap.h"
#include "Converter.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <set>
#include <tuple>

void GoBor(NodePtr root, std::unordered_map<uint16_t, std::vector<bool>> &code);

void Compress(std::istream &input_stream, std::ostream &output_stream, const std::string &file_name, bool is_last,
              BitWriter &writer);

struct Compare {
    bool
    operator()(const std::pair<uint16_t, std::vector<bool>> &a, const std::pair<uint16_t, std::vector<bool>> &b) const {
        if (a.second.size() != b.second.size()) {
            return a.second.size() < b.second.size();
        }
        return a.first < b.first;
    }
};