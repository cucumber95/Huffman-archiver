#include "BitWriter.h"

BitWriter::BitWriter(std::ostream &output_stream)
        : fout_(output_stream) {
}

void BitWriter::WriteBits(std::vector<bool> bits) {
    for (size_t i = 0; i < bits.size(); ++i) {
        if (buffer_size_ == 8) {
            Push();
        }

        buffer_ <<= 1;
        buffer_ |= bits[i];
        ++buffer_size_;
    }
}

void BitWriter::Push() {
    while (buffer_size_ < 8 && buffer_size_ != 0) {
        buffer_ <<= 1;
        ++buffer_size_;
    }

    char write = buffer_;
    fout_.write(&write, 1);
    buffer_ = 0;
    buffer_size_ = 0;
}



