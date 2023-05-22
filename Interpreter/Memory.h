#pragma once
#include <memory>
#include <vector>
#include "Types.h"
namespace Interpretator
{
    /*!
        Память
        \details Память 16 битных слов\n
        Хранит данные и команды.Количество ячеек памяти- 65535 \n
        см подробнее Memory_t,Memory_cmd
    */
    class Memory final
    {
        std::vector<Word_t> memory;
    public:
        Memory();
        ~Memory();
        /// Оператор доступа
        Word_t& operator[](Address_t address);

        Memory(const Memory&) = delete;
        Memory(const Memory&&) = delete;
        Memory& operator=(const Memory&) = delete;
        Memory& operator=(const Memory&&) = delete;
    };
}
