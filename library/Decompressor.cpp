#include "Decompressor.h"

void UpdateBor(std::vector<bool> &bits, uint16_t symbol, NodePtr node) {
    for (size_t i = 0; i < bits.size(); ++i) {
        if (bits[i] == 0) {
            if (node->left == nullptr) {
                node->left = AddNode();
            }
            node = node->left;
        } else {
            if (node->right == nullptr) {
                node->right = AddNode();
            }
            node = node->right;
        }
    }

    node->symbol = symbol;
}

uint16_t GetSymbol(BitReader &reader, NodePtr node) {
    std::vector<bool> bit;
    while (true) {
        bit = reader.ReadBits(1);
        if (bit[0] == 0) {
            if (node->left == nullptr) {
                throw std::exception();
            }
            node = node->left;
        } else {
            if (node->right == nullptr) {
                throw std::exception();
            }
            node = node->right;
        }
        if (node->left == nullptr && node->right == nullptr) {
            return node->symbol;
        }
    }
}

bool Decompress(BitReader &reader) {
    size_t symbols_count = ConvertVToInt16(reader.ReadBits(9));
    std::vector<uint16_t> symbols;
    for (size_t i = 0; i < symbols_count; ++i) {
        symbols.push_back(ConvertVToInt16(reader.ReadBits(9)));
        if (symbols.back() > 258) {
            throw std::exception();
        }
    }

    size_t max_size = 0;
    std::vector<size_t> count(1);
    for (size_t i = 1; symbols_count > 0; ++i) {
        max_size = i;
        size_t current_count = ConvertVToInt16(reader.ReadBits(9));
        symbols_count -= current_count;
        if (symbols_count < 0) {
            throw std::exception();
        }
        count.push_back(current_count);
    }

    std::vector<bool> cur_canonical_code;
    std::vector<std::vector<bool>> canonical_codes;
    for (size_t i = 1; i <= max_size; ++i) {
        while (cur_canonical_code.size() < i) {
            cur_canonical_code.push_back(0);
        }

        for (size_t j = 0; j < count[i]; ++j) {
            canonical_codes.push_back(cur_canonical_code);
            IncreaseVector(cur_canonical_code);
        }
    }

    NodePtr root = AddNode();
    for (size_t i = 0; i < symbols.size(); ++i) {
        UpdateBor(canonical_codes[i], symbols[i], root);
    }

    std::string file_name;
    while (true) {
        uint16_t symbol = GetSymbol(reader, root);
        if (symbol > 256) {
            throw std::exception();
        }

        if (symbol == 256) {
            break;
        }

        file_name += static_cast<char>(symbol);
    }

    std::ofstream fout(file_name, std::ios_base::binary);
    BitWriter writer(fout);
    while (true) {
        uint16_t symbol = GetSymbol(reader, root);
        if (symbol == 256) {
            throw std::exception();
        }

        if (symbol == 257) {
            writer.Push();
            return false;
        }

        if (symbol == 258) {
            writer.Push();
            return true;
        }

        writer.WriteBits(ConvertIntToV(symbol, 8));
    }
}

