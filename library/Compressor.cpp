#include "Compressor.h"

std::vector<bool> current_code;

void GoBor(NodePtr root, std::unordered_map<uint16_t, std::vector<bool>> &codes) {
    if (root->left == nullptr && root->right == nullptr) {
        codes[root->symbol] = current_code;
        return;
    }

    if (root->left != nullptr) {
        current_code.push_back(0);
        GoBor(root->left, codes);
        current_code.pop_back();
    }

    if (root->right != nullptr) {
        current_code.push_back(1);
        GoBor(root->right, codes);
        current_code.pop_back();
    }
}

void Compress(std::istream &input_stream, std::ostream &output_stream, const std::string &file_name, bool is_last,
              BitWriter &writer) {
    BitReader reader(input_stream);
    std::vector<bool> bits;
    std::vector<size_t> count(259);
    while (true) {
        try {
            bits = reader.ReadBits(8);
            uint8_t byte = ConvertVToInt8(bits);
            ++count[byte];
        } catch (std::exception &) {
            break;
        }
    }

    for (const u_char &symbol: file_name) {
        ++count[symbol];
    }

    ++count[256];
    ++count[257];
    ++count[258];
    Heap heap;
    for (uint16_t i = 0; i < 259; ++i) {
        if (!count[i]) {
            continue;
        }
        NodePtr temporary = AddNode(count[i], i);
        heap.Add(temporary);
    }

    while (heap.Size() > 1) {
        NodePtr min1 = heap.GetMin();
        NodePtr min2 = heap.GetMin();
        NodePtr temporary = AddNode(min1, min2);
        heap.Add(temporary);
    }

    NodePtr root = heap.GetMin();
    std::unordered_map<uint16_t, std::vector<bool>> codes;
    std::set<std::pair<uint16_t, std::vector<bool>>, Compare> sorted_codes;
    std::unordered_map<uint16_t, std::vector<bool>> canonical_codes;
    std::vector<uint16_t> symbols_order;
    GoBor(root, codes);
    for (auto&[symbol, symbol_code]: codes) {
        sorted_codes.insert({symbol, symbol_code});
    }

    std::vector<bool> cur_canonical_code;
    size_t max_length = 0;
    std::vector<size_t> cnt_length(100);
    for (auto&[symbol, symbol_code]: sorted_codes) {
        if (cur_canonical_code.empty()) {
            for (size_t i = 0; i < symbol_code.size(); ++i) {
                cur_canonical_code.push_back(0);
            }

            canonical_codes[symbol] = cur_canonical_code;
            symbols_order.push_back(symbol);
            max_length = cur_canonical_code.size();
            ++cnt_length[cur_canonical_code.size()];
            continue;
        }
        IncreaseVector(cur_canonical_code);
        while (cur_canonical_code.size() < symbol_code.size()) {
            cur_canonical_code.push_back(0);
        }

        canonical_codes[symbol] = cur_canonical_code;
        symbols_order.push_back(symbol);
        max_length = std::max(max_length, cur_canonical_code.size());
        ++cnt_length[cur_canonical_code.size()];
    }

    writer.WriteBits(ConvertIntToV(symbols_order.size(), 9));
    for (auto &symbol: symbols_order) {
        writer.WriteBits(ConvertIntToV(symbol, 9));
    }

    for (size_t i = 1; i <= max_length; ++i) {
        writer.WriteBits(ConvertIntToV(cnt_length[i], 9));
    }

    for (const u_char &symbol: file_name) {
        writer.WriteBits(canonical_codes[symbol]);
    }

    writer.WriteBits(canonical_codes[256]);
    reader.Reset();
    while (true) {
        try {
            bits = reader.ReadBits(8);
            uint8_t symbol = ConvertVToInt8(bits);
            writer.WriteBits(canonical_codes[symbol]);
        } catch (std::exception &) {
            break;
        }
    }

    if (is_last) {
        writer.WriteBits(canonical_codes[258]);
        writer.Push();
    } else {
        writer.WriteBits(canonical_codes[257]);
    }
}