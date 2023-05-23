#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <fstream>
#include "../CPU.h"
#include "TypesTranslator.h"
#include "../Extention.h"
/*
Формат бинарного файла.
-Адрес таблицы пер. имен
-Количество данных
-Стартовая метка( вход в программу)
-Данные по 2 слова
-Код - по 1 слову
-Таблица перемещающихся имен по 1 слову
*/
namespace Translator
{
    using namespace Interpretator;
    class Translator
    {
        std::unordered_map<std::string, std::tuple<int, int>> CommandsList = { // Команда асм , код операции в интер-ре, тип команды
            {"mov",std::tuple<int,int>(CPU::move,TypeCommand::Move)},
            {"lea",std::tuple<int,int>(CPU::move,TypeCommand::Lea)},
            {"add",std::tuple<int,int>(CPU::iAdd,TypeCommand::Math)},
            {"sub",std::tuple<int,int>(CPU::iSub,TypeCommand::Math)},
            {"mul",std::tuple<int,int>(CPU::iMul,TypeCommand::Math)},
            {"div",std::tuple<int,int>(CPU::iDiv,TypeCommand::Math)},
            {"fadd",std::tuple<int,int>(CPU::fAdd,TypeCommand::Math)},
            {"fsub",std::tuple<int,int>(CPU::fSub,TypeCommand::Math)},
            {"fmul",std::tuple<int,int>(CPU::fMul,TypeCommand::Math)},
            {"fdiv",std::tuple<int,int>(CPU::fDiv,TypeCommand::Math)},
            {"and",std::tuple<int,int>(CPU::And,TypeCommand::Math)},
            {"or",std::tuple<int,int>(CPU::Or,TypeCommand::Math)},
            {"not",std::tuple<int,int>(CPU::Not,TypeCommand::Math)},
            {"xor",std::tuple<int,int>(CPU::Xor,TypeCommand::Math)},
            {"shl",std::tuple<int,int>(CPU::shif_L,TypeCommand::Math)},
            {"shr",std::tuple<int,int>(CPU::shif_R,TypeCommand::Math)},
            {"input",std::tuple<int,int>(CPU::in,TypeCommand::Io)},
            {"output",std::tuple<int,int>(CPU::out,TypeCommand::Io)},
            {"cmp",std::tuple<int,int>(CPU::Icmp,TypeCommand::Cmp)},
            {"fcmp",std::tuple<int,int>(CPU::Fcmp,TypeCommand::Cmp)},
            {"jmp",std::tuple<int,int>(CPU::jmp,TypeCommand::Jumps)},
            {"je",std::tuple<int,int>(CPU::je,TypeCommand::Jumps)},
            {"jne",std::tuple<int,int>(CPU::jne,TypeCommand::Jumps)},
            {"jl",std::tuple<int,int>(CPU::jl,TypeCommand::Jumps)},
            {"jle",std::tuple<int,int>(CPU::jle,TypeCommand::Jumps)},
            {"jg",std::tuple<int,int>(CPU::jg,TypeCommand::Jumps)},
            {"jge",std::tuple<int,int>(CPU::jge,TypeCommand::Jumps)},
            {"jb",std::tuple<int,int>(CPU::jb,TypeCommand::Jumps)},
            {"jbe",std::tuple<int,int>(CPU::jbe,TypeCommand::Jumps)},
            {"ja",std::tuple<int,int>(CPU::ja,TypeCommand::Jumps)},
            {"jae",std::tuple<int,int>(CPU::jae,TypeCommand::Jumps)},
            {"call",std::tuple<int,int>(CPU::call,TypeCommand::Call)},
            {"ret",std::tuple<int,int>(CPU::ret,TypeCommand::WithoutParam)}
        };

        // Возвращает 0,если not exist
        std::unordered_map<std::string, uint16_t> Registers = { {"r1",1},{"r2",2},{"r3",3},{"r4",4},
                                                                {"r5",5},{"r6",6},{"r7",7},{"r8",8} };
        uint32_t globalAddress = 0; // Глобалльный адрес для трансляции имен 
        uint32_t startLabel = 0; // Адрес стартового входа в программу
        std::unordered_map<std::string, ElemNameTable> NameTable; // Таблица имен
        std::vector<Word_t> TranslatorMemory; // Буфер трансляции программы
        std::vector<uint16_t> tableMovingName; // Таблица перемещающихся имен
        std::string PathToDirectory;
        // Запись в буффер
        void WriteMem(Word_t value)
        {
            TranslatorMemory.push_back(value);
            globalAddress++;
        }
        //Запись перемещающегося имени
        void AddMovingName(size_t adress)
        {
            tableMovingName.push_back(adress);
        }
        //Тип операнда
        TypeOrerand Type(std::string str);
        // Парсинг операндов
        std::vector<std::string> ParseArguments(std::string str);

        std::vector<std::string> GetTokens(std::string str);
        // Загрузка адреса
        void Lea(uint16_t code, std::string arguments);
        // Перемещение
        void Move(uint16_t code, std::string arguments);
        // Переходы
        void Jump(uint16_t code, std::string arguments);
        //Арифметика
        void Math(uint16_t code, std::string arguments);
        // Сравнение
        void Cmp(uint16_t code, std::string arguments);
        // Вызов подпрограммы
        void Call(uint16_t code, std::string arguments);
        // Ввод-вывод
        void IO(uint16_t code, std::string str);
        // Парсингш команды
        void ParseCommand(std::tuple<int, int> cmd, std::string arguments);
        // Первый проход
        void FirstTranslate(std::stringstream& istr);
        // Первый проход секции кода
        void ParseCodeSection(std::stringstream& istr);

        std::vector<std::string> ParseArrayType(int type, std::string str);
        // Первый проход в секции данных
        void ParseDataSection(std::stringstream& istr);
        // Запись буфера в бинарнвй файл
        void WriteBinFile(std::string filename);

        void WriteTxtFile(std::string filename);
        // Второй проход -- основная трансляция
        void Parse(std::stringstream& istr);
    public:
        // Трансляция програмы
        void Translate(std::string filename);

        Translator(std::string directoryPath)
        {
            PathToDirectory = directoryPath;
        }
        ~Translator() {}
    };
}

