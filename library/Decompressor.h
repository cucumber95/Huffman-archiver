#pragma once

#include "BitReader.h"
#include "BitWriter.h"
#include "Converter.h"
#include "Bor.h"
#include "Heap.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

void UpdateBor(std::vector<bool> &bits, uint16_t symbol);

uint16_t GetSymbol(BitReader &reader, NodePtr node);

bool Decompress(BitReader &reader);