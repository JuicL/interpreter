#pragma once 
#include "../CPU.h"
#include "../Types.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using Interpretator::CPU;
class Loader
{
public:
	Loader();
	~Loader();
    uint16_t LoadAdress;
	std::vector<std::string> split(const std::string& s, char symb);
    // Загрузка из текстового файла
    bool load(CPU& cpu, const std::string& filename);
    /// Загрузка памяти из бинарного файла 
	bool LoadFromBinary(CPU& cpu, uint32_t startAdr, const std::string& filename);
};




