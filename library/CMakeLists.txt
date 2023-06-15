cmake_minimum_required(VERSION 3.16)

set(SRC_FILES BitReader.cpp Bor.cpp Heap.cpp BitWriter.cpp Converter.cpp Compressor.cpp Decompressor.cpp)

add_library(archiver_library ${SRC_FILES})
target_include_directories(archiver_library PUBLIC ./)