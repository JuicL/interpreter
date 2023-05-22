#include "Loader.h"
#include <iostream>
#include <fstream>
using std::ios;

Loader::Loader()
{
}

Loader::~Loader()
{
}
/// Загрузка памяти из текстового файла
bool Loader::load(CPU& cpu, const std::string& filename)
{
    std::ifstream file;
    file.open(filename);

    if (!file.is_open())
    {
        std::cout << "File [ isn't open ]\n";
        return false;
    }

    Address_t addr = 0;
    char symb = ' ';
    while (symb != 'e')
    {
        std::string str;
        getline(file, str);
        std::istringstream istr(str);
        istr >> symb;
        switch (symb)
        {
        case 'a':
        {
            istr >> addr;
            break;
        }
        case 'i':
        {
            Int32_t digit;
            istr >> digit.int32;
            for (int i = 0; i < 2; ++i)
            {
                cpu.memory[addr]= digit.word[i];
                ++addr;
            }
            break;
        }
        case 'u':
        {
            Int32_t digit;
            istr >> digit.uint32;
            for (int i = 0; i < 2; ++i)
            {
                cpu.memory[addr] = digit.word[i];
                ++addr;
            }
            break;
        }
        case 'f':
        {
            Flt32_t digit;
            istr >> digit.flt32;
            for (int i = 0; i < 2; ++i)
            {
                cpu.memory[addr] = digit.word[i];
                ++addr;
            }
            break;
        }
        case 'c':
        {
            Cmd32_t command{};
            std::vector<std::string> vector1 = split(istr.str(), ' ');
            vector1.erase(vector1.begin());
            for (size_t i = 0; i < vector1.size(); ++i)
            {
                if (vector1[i].empty())
                {
                    vector1.erase(vector1.begin() + i, vector1.end());
                    break;
                }
            }
            command.cmd16.code = uint8_t(atoi(vector1[0].c_str()));
            command.cmd16.r1 = uint8_t(atoi(vector1[1].c_str()));
            command.cmd16.r2 = uint8_t(atoi(vector1[2].c_str()));
            command.cmd16.r3 = uint8_t(atoi(vector1[3].c_str()));

            cpu.memory[addr] = command.cmd16.data;         // Запись команды в память
            ++addr;

            if (vector1.size() == 5)
            {
                Address_t temp_adress;
                temp_adress = Address_t(atoi(vector1[4].c_str()));
                cpu.memory[addr] = temp_adress;
                ++addr;
            }
            break;
        }
        case 'e':
        {
            Cmd16_t command{};
            command.code = 0;
            command.r1 = 5;
            cpu.memory[addr]= command.data;
            uint16_t IP;
            istr >> IP;
            cpu.psw.SetIP(IP);
            break;
        }
        }
    }
    file.clear();
    file.close();
    return true;
}

bool Loader::LoadFromBinary(CPU& cpu, uint32_t startAdr, const std::string& filename)
{
    std::ifstream file(filename, ios::binary | ios::in);
    if (!file.is_open())
    {
        std::cout << "File [ isn't open ]\n";
        return false;
    }
    uint32_t countData, movingTable, addr, offsetForMovingname;
    file.read((char*)&movingTable, sizeof(movingTable));
    file.read((char*)&countData, sizeof(countData));
    file.read((char*)&addr, sizeof(addr));
    //offsetForMovingname = addr;
    cpu.psw.ip = addr + startAdr;
    Data32_t data;
    uint32_t currAdress = startAdr;
    for (; currAdress < countData; currAdress += 2)
    {
        file.read((char*)&data.int32.int32, sizeof(data.int32.int32));
        std::memcpy(&cpu.memory[currAdress], &data.int32.word[0], sizeof(Word_t) * 2);
    }
    Cmd16_t code;
    for (size_t i = countData; i < movingTable; i++, currAdress++)
    {
        file.read((char*)&code, sizeof(code));
        cpu.memory[currAdress] = code.data;
    }
    code.code = 0;
    cpu.memory[currAdress] = code.data;
    uint16_t movingName;
    while (file.read((char*)&movingName, sizeof(movingName)))
    {
        cpu.memory[movingName + startAdr] += startAdr;
    }
    return true;
}



std::vector<std::string> Loader::split(const std::string& s, char symb)
{
        std::vector<std::string> elems;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, symb))
        {
            elems.push_back(item);
        }
        return elems;
}
