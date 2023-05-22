#include <algorithm>
#include <exception>
#include "Translator.h"
#include "..\ParseIndirect.h"
#include "..\ArifmeticParser.h"
#include "..\Atoi\Atoi.h"
namespace Translator
{


    /*
    Вещественные числа
    Операции с []
    call
    */
    // Тип операнда
    TypeOrerand Translator::Type(std::string str)
    {
        std::string action = "+-/*";
        if (Registers.find(str) != Registers.end())
        {
            return TypeOrerand::Register;
        }
        // Косвенное обращение формата [Register + offset]
        if (str.find('[') != std::string::npos and str.find(']') != std::string::npos)
        {
            return TypeOrerand::Indirect;
        }
        // Косвенное обращение,только без "[]" -- Register + offset
        int indexReg = str.find("r");
        if (indexReg != std::string::npos)
        {
            if (Registers.find("r" + str[indexReg + 1]) != Registers.end()) // Проверка существует ли такой регистр
            {
                if (str.find('+') != std::string::npos or
                    str.find('-') != std::string::npos)
                {
                    return TypeOrerand::Indirect;
                }
            }
            else
            {//exeption
            }
        }
        // Арифемтика
        for (char ch : action)
        {
            if (str.find(ch) != std::string::npos)
            {
                return TypeOrerand::Const;
            }
        }
        // Константа
        int resTemp;
        Atoi at;
        if (at.ParseNumber(str, resTemp))
        {
            return TypeOrerand::Const;
        }
        // Переменная
        bool variable = true;
        if (isdigit(str[0])) variable = false;
        for (size_t i = 0; i < str.length() && variable; i++)
        {
            if (!isalpha(str[i]) && !isdigit(str[i]) && str[i] != '_')
                variable = false;
        }
        //if (NameTable.find(str) == NameTable.end()) // Проверка присутствует ли такая переменная
        if (variable) return Variable;
        if (str.find('#') != std::string::npos) return Special;
        throw std::logic_error(str + " Неизвестный тип аргумента");
    }
    // Удалить из строки комментарий
    void EraseCommentFromLine(std::string& str)
    {
        int indexComment = str.find(';');
        if (indexComment != std::string::npos)
        {
            str = str.substr(0, indexComment);
        }
    }
    // Парсинг аргументов
    std::vector<std::string> Translator::ParseArguments(std::string str)
    {
        str.erase(remove_if(str.begin(), str.end(), isspace), str.end()); // Удаление пробелов
        std::vector<std::string> args = split(str, ','); // Разделить строку по ','
        args.erase(std::remove(args.begin(), args.end(), ","), args.end()); // Удалить из массива элементы = ','
        return args;
    }

    std::vector<std::string> Translator::GetTokens(std::string str)
    {
        std::stringstream tempStream(str);
        std::vector<std::string> splits;

        while (!tempStream.eof())
        {
            std::string temp;
            tempStream >> temp;
            if (!temp.empty())
                splits.push_back(temp);
        }
        return splits;
    }
    // Парсинг загрузки адреса
    void Translator::Lea(uint16_t code, std::string arguments)
    {
        Memory_cmd temp;
        temp.cmd.code = code;
        std::vector<std::string> args = ParseArguments(arguments);
        if (args.size() != 2)
            throw std::logic_error("Ожидалось 2 аргумента");
        ParseIndirect parseInderect;
        std::pair<int, int> inderectResult;
        if (Type(args[0]) != TypeOrerand::Register)throw std::logic_error("Ожидался регистр");
        switch (Type(args[1]))
        {
        case TypeOrerand::Variable:
            if (NameTable.find(args[1]) == NameTable.end())
                throw std::logic_error(args[1] + " не определена");
            temp.cmd.r1 = (uint16_t)Registers[args[0]] - 1;
            temp.cmd.r2 = 2;
            WriteMem(temp.word);
            temp.addres = NameTable[args[1]].address;
            AddMovingName(globalAddress);
            WriteMem(temp.word);
            break;
        case TypeOrerand::Indirect:
            inderectResult = parseInderect.ToParseInderect(args[1]);
            temp.cmd.r2 = 6;
            temp.cmd.r3 = inderectResult.first;
            WriteMem(temp.word);
            temp.addres = inderectResult.second;
            WriteMem(temp.word);
        default:
            throw std::logic_error("Неверные типы аргументов");
            break;
        }
    }
    // Todo проверить mode r2
    // Парсинг команд перемещения
    void Translator::Move(uint16_t code, std::string arguments)
    {
        Memory_cmd temp;
        std::vector<std::string> args = ParseArguments(arguments);
        if (args.size() != 2)
            throw std::logic_error("Ожидалось 2 аргумента");
        ParseIndirect parseInderect;
        std::pair<int, int> inderectResult;

        temp.cmd.code = code;
        int arg1 = Type(args[0]),
            arg2 = Type(args[1]);
        if (arg1 != TypeOrerand::Register && arg2 != TypeOrerand::Register)
            throw std::logic_error("Недопустимые типы агрументов"); // Память-память нельзя!

        ArifParser parser;
        Atoi at;
        bool firstIsRegister = arg1 == TypeOrerand::Register;
        temp.cmd.r1 = Registers[args[!firstIsRegister]] - 1;
        int secondOperand = (arg1 == TypeOrerand::Register) ? arg2 : arg1;
        switch (secondOperand)
        {
        case TypeOrerand::Variable:
            if (NameTable.find(args[firstIsRegister]) == NameTable.end())
                throw std::logic_error(args[firstIsRegister] + " не определена");
            temp.cmd.r2 = (firstIsRegister) ? 1 : 5;
            WriteMem(temp.word);
            temp.addres = NameTable[args[firstIsRegister]].address;
            AddMovingName(globalAddress);
            WriteMem(temp.word);
            break;
        case TypeOrerand::Register:
            temp.cmd.r2 = 0;
            temp.cmd.r3 = Registers[args[firstIsRegister]] - 1;
            WriteMem(temp.word);
            break;
        case TypeOrerand::Const:
            if (!firstIsRegister)throw std::logic_error("Попытка загрузить значение регистра в число");
            temp.cmd.r2 = 2;
            WriteMem(temp.word);
            //int resTemp;
            //at.ParseNumber(args[firstIsRegister], resTemp);
            temp.addres = (Address_t)parser.Result(args[firstIsRegister].c_str());
            WriteMem(temp.word);
            break;
        case TypeOrerand::Indirect:
            inderectResult = parseInderect.ToParseInderect(args[firstIsRegister]);
            temp.cmd.r2 = (firstIsRegister) ? 3 : 7;
            temp.cmd.r3 = inderectResult.first;
            WriteMem(temp.word);
            temp.addres = inderectResult.second;
            WriteMem(temp.word);
            break;
        default:
            throw std::logic_error("Недопустимый тип параметров");
            break;
        }
    }

    void Translator::Call(uint16_t code, std::string arguments)
    {
        std::vector<std::string> args = ParseArguments(arguments);
        if (args.size() != 1)
            throw std::logic_error("Ожидался 1 аргумент");
        int argumentType = Type(args[0]);
        if (argumentType != TypeOrerand::Variable) throw std::logic_error("Ожидалась метка");
        if (NameTable.find(args[0]) == NameTable.end()) throw std::logic_error(args[0] + " не определена");
        if (NameTable[args[0]].type != 0)
            throw std::logic_error("Ожидалась метка а не переменная");
        Memory_cmd temp;
        temp.cmd = { code,7,0,0 };
        WriteMem(temp.word);
        ElemNameTable variable = NameTable[args[0]];
        temp.addres = variable.address;
        AddMovingName(globalAddress);
        WriteMem(temp.word);
    }
    // Парсинг команд перехода
    void Translator::Jump(uint16_t code, std::string arguments)
    {
        std::vector<std::string> args = ParseArguments(arguments);
        if (args.size() != 1)
            throw std::logic_error("Ожидался 1 аргумент");

        int argumentType = Type(args[0]);
        switch (argumentType)
        {
        case TypeOrerand::Variable: // Переход на метку
        {
            if (NameTable.find(args[0]) == NameTable.end()) throw std::logic_error(args[0] + " не определена");
            ElemNameTable variable = NameTable[args[0]];
            if (variable.type == 0) // Является ли имя меткой
            {
                Memory_cmd temp;
                temp.cmd = { code,0b000,0b001,0 };
                WriteMem(temp.word);
                temp.addres = variable.address;
                AddMovingName(globalAddress);
                WriteMem(temp.word);
            }
            else
                throw std::logic_error(args[0] + " не является меткой");
            break;
        }
        case TypeOrerand::Register: // Косвенный
        {
            Memory_cmd temp;
            temp.cmd = { code,0,0,(uint16_t)(Registers[args[0]] - 1) };
            WriteMem(temp.word);
            break;
        }
        case TypeOrerand::Indirect: // Косвенная со смещением( без "[]" -- Register +(-) offset
        {
            ParseIndirect parseInderect;
            std::pair<int, int> res = parseInderect.ToParseInderect(args[0]);
            Memory_cmd temp;
            temp.cmd = { code,0,0b011,(uint16_t)res.first };
            WriteMem(temp.word);
            temp.addres = res.second;
            WriteMem(temp.word);
            break;
        }
        default:
            throw std::logic_error("Недоступный тип аргументов");
            break;
        }
    }
    // Парсинг арифметических команд
    void Translator::Math(uint16_t code, std::string arguments)
    {
        std::vector<std::string> args = ParseArguments(arguments);
        if (code == CPU::Not)
        {
            if (args.size() != 2) throw std::logic_error("Ожидалось 2 аргумента");
        }
        else
        {
            if (args.size() != 3) throw std::logic_error("Ожидалось 3 аргумента");
        }

        for (size_t i = 0; i < args.size(); i++)
        {
            if (Type(args[i]) != TypeOrerand::Register) throw std::logic_error("Арифметические команды только на регистрах");
        }
        Memory_cmd temp;
        if (code == CPU::Not)
            temp.cmd = { code, (uint16_t)(Registers[args[0]] - 1), (uint16_t)(Registers[args[1]] - 1), 0 };
        else
            temp.cmd = { code, (uint16_t)(Registers[args[0]] - 1), (uint16_t)(Registers[args[1]] - 1), (uint16_t)(Registers[args[2]] - 1) };
        WriteMem(temp.word);
    }

    void Translator::Cmp(uint16_t code, std::string arguments)
    {
        std::vector<std::string> args = ParseArguments(arguments);
        if (args.size() < 2) throw std::logic_error("Ожидалось 2 аргумента");
        for (size_t i = 0; i < args.size(); i++)
        {
            if (Type(args[i]) != TypeOrerand::Register) throw std::logic_error("Команды сравнения только на регистрах");
        }
        Memory_cmd temp;
        temp.cmd = { code, 0, (uint16_t)(Registers[args[0]] - 1), (uint16_t)(Registers[args[1]] - 1) };
        WriteMem(temp.word);
    }

    // Парсинг команд ввода и вывода
    void Translator::IO(uint16_t code, std::string arguments)
    {
        std::vector<std::string> args = ParseArguments(arguments);
        if (args.size() != 2) throw std::logic_error("Ожидалось 2 аргумента");
        if (Type(args[0]) != TypeOrerand::Register) throw std::logic_error(args[0] + " ожидался регистр");
        if (Type(args[1]) != TypeOrerand::Special) throw std::logic_error(args[1] + " Ожидалась конструкция #");
        uint8_t digitParam{ 0 };
        if (isNumber(args[1].substr(1)))
        {
            int digitParam = std::atoi(args[1].substr(1).c_str());
            if (digitParam < 0 || digitParam > 2)
                throw std::logic_error(args[1] + " Ожидалась конструкция #<0..2>");
        }
        else
            throw std::logic_error(args[1] + " Ожидалась конструкция #<0..2>");

        Memory_cmd temp;
        temp.cmd = { code, (uint16_t)(Registers[args[0]] - 1), (uint16_t)std::atoi(args[1].substr(1).c_str()), 0 };
        WriteMem(temp.word);
    }
    // Селектор функции-обработчика для парсинга команды
    void Translator::ParseCommand(std::tuple<int, int> cmd, std::string arguments)
    {
        switch (std::get<1>(cmd))
        {
        case TypeCommand::Move:
        {
            Move(std::get<0>(cmd), arguments); break;
        }
        case TypeCommand::Lea:
        {
            Lea(std::get<0>(cmd), arguments); break;
        }
        case TypeCommand::Jumps:
        {
            Jump(std::get<0>(cmd), arguments); break;
        }
        case TypeCommand::Math:
        {
            Math(std::get<0>(cmd), arguments); break;
        }
        case TypeCommand::Io:
        {
            IO(std::get<0>(cmd), arguments); break;
        }
        case TypeCommand::Cmp:
        {
            Cmp(std::get<0>(cmd), arguments);
            break;
        }
        case TypeCommand::Call:
        {
            Call(std::get<0>(cmd), arguments);
            break;
        }
        case TypeCommand::WithoutParam:
        {
            Memory_cmd temp;
            temp.cmd = { (uint16_t)std::get<0>(cmd),7,0,0 };
            WriteMem(temp.word);
            break;
        }
        default:
            throw std::logic_error("Неизвестная команда");
            break;
        }

    }
    // Транслирвание asm файла
    void Translator::Translate(std::string filename)
    {
        std::ifstream file;
        file.open(filename);
        if (!file.is_open())
        {
            std::cout << "File [ isn't open ]\n";
            return;
        }
        std::stringstream istr;
        istr << file.rdbuf();
        FirstTranslate(istr);
        file.seekg(0);
        istr.clear();
        istr << file.rdbuf();
        globalAddress = std::count_if(NameTable.begin(), NameTable.end(), [](std::pair<std::string, ElemNameTable> elem) {return elem.second.type != 0; }) * 2;
        Parse(istr);
    }

    // Первый проход -- сбор имен и запись переменных
    void Translator::FirstTranslate(std::stringstream& istr)
    {
        bool dataSection = false;
        bool codeSection = false;
        std::string str;
        istr >> str;
        while (!istr.eof())
        {
            EraseCommentFromLine(str);
            if (str == ".data")
            {
                std::getline(istr, str, '\n');
                if (codeSection) throw std::logic_error("Секция данных после секции кода");
                if (dataSection) throw std::logic_error("Опеределение секции данных уже встречалось");
                dataSection = true;
                ParseDataSection(istr); // Сбор имен переменных
            }
            else if (str == ".code")
            {
                std::getline(istr, str, '\n');
                if (codeSection) throw std::logic_error("Опеределение секции кода уже встречалось");
                codeSection = true;
                ParseCodeSection(istr); // Сбор имен меток и метки start
            }
            else
            {
                std::getline(istr, str, '\n');
            }
            istr >> str;
        }
        if (!dataSection) throw std::logic_error("Отсутствует секция данных");
        if (!codeSection) throw std::logic_error("Отсутствует секция кода");

    }
    // Первый проход секции кода
    void Translator::ParseCodeSection(std::stringstream& istr)
    {
        bool isstartLabel = false;
        std::string nameStartLabel;
        std::string str;
        istr >> str;
        while (str.find(".data") == std::string::npos && !istr.eof())
        {
            if (str.find(';') != std::string::npos) // Пропуск комментария
            {
                std::getline(istr, str, '\n');
            }
            else
            {
                if (str == "start") // Точка входа в программу
                {
                    if (isstartLabel)
                        throw std::logic_error("Переопределение стартовой метки");
                    std::getline(istr, str, '\n');
                    EraseCommentFromLine(str);
                    std::vector<std::string> temp = GetTokens(str);
                    if (temp.size() != 1)
                        throw std::logic_error("Ошибка определения стартовой метки(встретилось больше 1 токена");
                    nameStartLabel = temp[0];
                    isstartLabel = true;
                }
                else
                {
                    int indexEndLabel = str.find(':'); // Метка
                    if (indexEndLabel != std::string::npos) // Добавление метки в таблицу имен
                    {
                        str = str.substr(0, indexEndLabel);
                        ElemNameTable label = { 0,globalAddress };
                        if (NameTable.find(str) != NameTable.end())
                            throw std::logic_error(str + " уже определена");
                        if (str.empty())
                            throw std::logic_error("Ошибка синтаксиса метки.Ожидалась конструкция <метка>:");
                        NameTable.insert(std::make_pair(str, label));
                    }
                    else // Если текущая строка содержит код,тогда просто считам адрес
                    {
                        std::string line;
                        std::getline(istr, line, '\n');
                        EraseCommentFromLine(line);
                        std::vector<std::string> args = ParseArguments(line);
                        for (size_t i = 0; i < args.size(); i++)
                        {
                            int typeOp = Type(args[i]);
                            if (typeOp == TypeOrerand::Indirect || typeOp == TypeOrerand::Variable || typeOp == TypeOrerand::Const)// Тимы операндов, которые занимают дополнительную ячейку памяти ВМ
                            {
                                globalAddress++;
                                break; // В команде не может быть более 1 адреса
                            }
                        }
                        globalAddress++;
                    }
                }
            }
            istr >> str;
        }
        if (!isstartLabel) throw std::logic_error("Стартовая метка не найдена");
        if (NameTable.find(nameStartLabel) != NameTable.end())
        {
            if (NameTable[nameStartLabel].type != 0) throw std::logic_error(nameStartLabel + "не является меткой");
            startLabel = NameTable[nameStartLabel].address;
        }
        else throw std::logic_error("Метка " + nameStartLabel + " не определена");
    }

    std::vector<std::string> Translator::ParseArrayType(int type, std::string str)
    {
        std::vector<std::string> res;
        std::vector<std::string> splits = GetTokens(str);
        if (str.find(',') != std::string::npos)
        {
            res = ParseArguments(str);
            if (res.size() == 0)
                throw std::logic_error(" Ошибка списка инициализации");
        }
        else if (str.find('dup') != std::string::npos)
        {
            if (splits.size() != 3 && !isNumber(splits[0]) && !isNumber(splits[2]) && splits[1] != "dup")
                throw std::logic_error(" Ожидалась конструкция <число> dup <число>");
            int size = std::atoi(splits[0].c_str());
            for (size_t i = 0; i < size; i++)
            {
                res.push_back(splits[2]);
            }
        }
        else
        {
            if (splits.size() > 1)
                throw std::logic_error("Ожидалось 1 значение");
            res = splits;
        }
        return res;
    }

    // Первый проход для секции данных
    void Translator::ParseDataSection(std::stringstream& istr)
    {
        enum DataType { Integer = 1, Double = 2, Array = 3 };
        std::unordered_map<std::string, int> type = { {"int",Integer},{"flt",Double},{"dup",Array} };
        std::string str;
        std::streampos pos;
        std::getline(istr, str, '\n');
        EraseCommentFromLine(str);
        while (str.find(".code") == std::string::npos && !istr.eof())
        {
            std::stringstream tempStream(str);
            std::vector<std::string> splits;
            for (size_t i = 0; i < 2 && !tempStream.eof(); i++)
            {
                std::string temp;
                tempStream >> temp;
                if (!temp.empty()) splits.push_back(temp);
            }

            if (splits.size() != 0)
            {
                if (splits.size() < 2)  throw std::logic_error("Ожидалось минимум 2 аргумента");
                if (Type(splits[0]) != TypeOrerand::Variable)
                    throw std::logic_error(splits[0] + " неверный формат имени переменной");
                if (NameTable.find(splits[0]) != NameTable.end())
                    throw std::logic_error(splits[0] + " уже определена");
                if (type.find(splits[1]) == type.end())
                    throw std::logic_error("Нераспознанный тип");
                ElemNameTable variable = { type[splits[1]], globalAddress };
                std::string initialValues;
                std::getline(tempStream, initialValues, '\n');
                std::vector<std::string> values = ParseArrayType(type[splits[1]], initialValues);
                for (size_t i = 0; i < values.size(); i++)
                {
                    if (type[splits[1]] == Integer) // Целое
                    {
                        Atoi at;
                        int result;
                        if (at.ParseNumber(values[i], result))
                        {
                            Data32_t data;
                            data.int32.int32 = result;
                            variable.value.push_back(data);
                        }
                        else
                            throw std::logic_error("Нераспознанный тип присваемого значения");
                    }
                    else if (type[splits[1]] == Double) // Вещественное
                    {
                        Data32_t data;
                        data.flt.flt32 = atof(values[i].c_str());
                        variable.value.push_back(data);
                    }
                }
                if (variable.value.size() == 0)
                {
                    Data32_t data;
                    data.int32.int32 = 0;
                    variable.value.push_back(data);
                }
                globalAddress += variable.value.size() * 2;
                NameTable.insert(std::make_pair(splits[0], variable)); // Добавить метку в таблицу имен
            }

            pos = istr.tellg();
            std::getline(istr, str, '\n');
            EraseCommentFromLine(str);
        }
        if (str.find(".code") != std::string::npos)
        {
            istr.seekg(pos);
        }
    }

    // Второй проход, основная трансляция
    void Translator::Parse(std::stringstream& istr)
    {
        std::string str;
        do
        {
            std::getline(istr, str, '\n');
            EraseCommentFromLine(str);
        } while (str.find(".code") == std::string::npos);

        istr >> str;
        while (!istr.eof())// Транслируем секцию кода
        {
            if (str.find(";") != std::string::npos) // Пропуск комменатриев,т.к считываение идет по слову из строки
            {
                std::getline(istr, str, '\n');
            }
            else if (str == "start")
            {
                std::getline(istr, str, '\n');
            }
            else if (str.find(":") == std::string::npos) // Пропуск всех меток И проверка является ли слово мнемоникой команды
            {
                if (CommandsList.find(str) == CommandsList.end())
                {
                    throw std::logic_error("Команда " + str + " не определена");
                }
                std::string arguments;
                std::getline(istr, arguments, '\n');// Получить строчку аргументов
                EraseCommentFromLine(arguments);// Очищаем строку от комментария
                try
                {
                    ParseCommand(CommandsList[str], arguments); // Парсинг команды
                }
                catch (const std::exception& e)
                {
                    throw std::logic_error(str + " " + arguments + "\n" + e.what());
                }
            }
            istr >> str;
        }
        WriteBinFile(PathToDirectory + "testbin.bin"); // Запись в бинарный файл
        WriteTxtFile(PathToDirectory + "DebbugInfo.txt");
    }

    // Запись отранслированной программы в бинарный файл
    void Translator::WriteBinFile(std::string filename)
    {
        // Адрес таблицы tableMovingName = Данные(по 4 байта) + код(по 2 байта)
        uint32_t countData{ 0 };
        std::vector<ElemNameTable> correctData;
        for (auto el : NameTable)
        {
            if (el.second.type != 0)
                correctData.push_back(el.second);
        }
        for (size_t i = 0; i < correctData.size(); i++)
        {
            countData += correctData[i].value.size() * 2;
        }

        uint32_t movingTable = countData + TranslatorMemory.size();// Расположение таблицы перемещающих имен
        std::ofstream file(filename, std::ios::binary);
        if (!file)
        {
            throw std::logic_error("Ошибка записи bin файла");
        }
        file.write((char*)&movingTable, sizeof(movingTable));
        file.write((char*)&countData, sizeof(countData));
        file.write((char*)&startLabel, sizeof(startLabel)); // Стартовое IP
        std::sort(correctData.begin(), correctData.end(), [](const ElemNameTable& lhs, const ElemNameTable rhs) {
            return lhs.address < rhs.address;
        });
        for (size_t i = 0; i < correctData.size(); i++)
        {
            for (size_t j = 0; j < correctData[i].value.size(); j++)
            {
                file.write((char*)&correctData[i].value[j].int32.int32, sizeof(correctData[j].value[j].int32.int32));
            }
        }
        for (Word_t code : TranslatorMemory) // Запись команд
        {
            file.write((char*)&code, sizeof(code));
        }
        for (uint16_t movingName : tableMovingName) // Таблица перемещающихся имен
        {
            file.write((char*)&movingName, sizeof(movingName));
        }
        file.close();
    }

    void Translator::WriteTxtFile(std::string filename)
    {
        std::ofstream out;          // поток для записи
        out.open(filename); // окрываем файл для записи
        if (out.is_open())
        {
            out << "Data" << std::endl;
            int adress = 0;
            std::vector<ElemNameTable> correctData;
            for (auto el : NameTable)
            {
                if (el.second.type != 0)
                    correctData.push_back(el.second);
            }
            std::sort(correctData.begin(), correctData.end(), [](const ElemNameTable& lhs, const ElemNameTable rhs) {
                return lhs.address < rhs.address;
            });

            for (ElemNameTable elem : correctData)
            {
                for (Data32_t dataElem : elem.value)
                {
                    out << std::to_string(adress) + "- "
                        << std::to_string(dataElem.int32.int32) + " " <<
                        std::to_string(dataElem.int32.uint32) + " " <<
                        std::to_string(dataElem.flt.flt32) << std::endl;
                    adress += 2;
                }
            }


            out << "Code" << std::endl;
            for (Word_t code : TranslatorMemory) // Запись команд
            {
                Cmd16_t temp;
                temp.data = code;
                out << std::to_string(adress) + "- "
                    << std::to_string(temp.code) + " "
                    << std::to_string(temp.r1) + " "
                    << std::to_string(temp.r2) + " "
                    << std::to_string(temp.r3) + "( "
                    << std::to_string(temp.data) + " )" << std::endl;
                adress++;
            }
        }
    }

}
