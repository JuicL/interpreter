#include "Memory.h"
namespace Interpretator
{
    Memory::Memory()
    {
        memory.resize(std::numeric_limits<uint16_t>::max());
        //std::make_unique<Word_t[]>(std::numeric_limits<uint16_t>::max());
    }

    Memory::~Memory() {}

    Word_t& Memory::operator[](Address_t address)
    {
        return memory[address];
    }
}
