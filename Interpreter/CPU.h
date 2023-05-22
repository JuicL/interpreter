#pragma once
#include <cinttypes>
#include <iostream>
#include <utility>
#include <cinttypes>
#include <vector>
#include "Types.h"
#include "Memory.h"
namespace Interpretator
{
    class CPU final
    {
        std::vector<std::unique_ptr<class Command>> _command;
        usual_cmd_t _cmd{};                                     /// Текущая выполняемая команда
    public:
        PSW psw{};                 /// PSW = IP + FLAGS
        class Memory memory;       /// Память
        Register_t reg{};          /// Регистры общего назначения
        bool isLongCmd;            /// Флаг длинной команды

        CPU();
        ~CPU();

        CPU(const CPU&) = delete;
        CPU(const CPU&&) = delete;
        CPU& operator=(const CPU&) = delete;
        CPU& operator=(const CPU&&) = delete;

        inline usual_cmd_t GetCmd() { return _cmd; }

        void Reset() noexcept;              /// Перезагрузка машины
        void Run() noexcept;                /// Старт машины  
        void LoadCurCommand() noexcept;     /// Загрузка текущей команды   

        /// Коды операций
        enum Operations : uint8_t
        {
            stop = 0,       ///< Остановка работы процессора
            move = 1,       ///< Пересылка данных

            iAdd = 2,       ///< Сложение целых чисел
            iSub = 3,       ///< Вычитание целых чисел
            iMul = 4,       ///< Умножение целых чисел
            iDiv = 5,       ///< Деление целых чисел

            fAdd = 6,       ///< Сложение дробных чисел
            fSub = 7,       ///< Вычитание дробных чисел
            fMul = 8,       ///< Умножение дробных чисел
            fDiv = 9,       ///< Деление дробных чисел

            And = 10,		///< Битовое И
            Or = 11,		///< Битовое ИЛИ
            Not = 12,		///< Битовое НЕ
            Xor = 13,
            shif_L = 14,    ///< Битовый сдвиг влево
            shif_R = 15,    ///< битовый сдвиг вправо

            in = 16,        ///< Ввод
            out = 17,       ///< Вывод

            Icmp = 18,      ///< Сравнение целых
            Fcmp = 19,      ///< Сравнение дробных

            jmp = 20,       ///< Прямой безусловный переход
            je = 21,        ///< Условный переход если равно
            jne = 22,       ///< Условный переход если не равно

            jl = 23,        ///< Условный переход если меньше
            jle = 24,       ///< Условный переход если меньше или равно
            jg = 25,        ///< Условный переход если больше
            jge = 26,       ///< Условный переход если больше или равно

            jb = 27,        ///< Условный переход если ниже
            jbe = 28,       ///< Условный переход если ниже или равно
            ja = 29,        ///< Условный переход если выше
            jae = 30,       ///< Условный переход если ниже или равно

            call = 31,       ///< Вызов подпрограммы
            ret = 32,       ///< Возврат из подпрограммы

            end             ///<
        };
        /// Режим ввода вывода
        enum Io_mode : uint8_t
        {
            ioInt,      ///< IO целых знаковых
            ioUint,     ///< IO целых беззнаковых
            ioFloat,    ///< IO дробных
        };
    };
}
