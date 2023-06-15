#include "Compressor.h"
#include "Decompressor.h"

#include <iostream>
#include <string>

const std::string HELP = "archiver -c archive_name file1 [file2 ...] - заархивировать файлы fil1, file2, ... и сохранить результат в файл archive_name.\n"
                         "archiver -d archive_name - разархивировать файлы из архива archive_name и положить в текущую директорию.\n"
                         "archiver -h - вывести справку по использованию программы.\n";

int main(int args, char *argv[]) {
    if (args < 2) {
        std::cerr << "Wrong arguments\n";
        return 0;
    }
    std::string command_type(argv[1]);
    if (command_type == "-c") {
        if (args < 4) {
            std::cerr << "Wrong arguments\n";
            return 0;
        }
        std::ofstream fout(argv[2], std::ios_base::binary);
        BitWriter writer(fout);
        for (size_t i = 3; i < args; ++i) {
            std::ifstream fin(argv[i], std::ios_base::binary);
            if (!fin.is_open()) {
                std::cerr << "Can't open file\n";
                return 0;
            }
            Compress(fin, fout, argv[i], i == args - 1, writer);
        }
    } else {
        if (command_type == "-d") {
            if (args != 3) {
                std::cerr << "Wrong arguments\n";
                return 0;
            }
            std::ifstream fin(argv[2], std::ios_base::binary);
            BitReader reader(fin);
            try {
                while (true) {
                    bool is_last = Decompress(reader);
                    if (is_last) {
                        break;
                    }
                }
            } catch (std::exception &) {
                std::cerr << "Something wrong with file\n";
                return 0;
            }
        } else {
            if (command_type == "-h") {
                std::cerr << HELP;
                return 0;
            } else {
                std::cerr << "Wrong arguments\n";
                return 0;
            }
        }
    }
}
