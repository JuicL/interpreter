#include <sstream>
#include <iomanip>
#include <bitset>
#include "Debuger.h"
#include "Extention.h"
#include "Loader/Loader.h"
std::string IntToHex(int i)
{
    std::stringstream stream;
    stream << "0x"
        << std::setfill('0') << std::setw(sizeof(int) * 2)
        << std::hex << i;
    return stream.str();
}
namespace Debuger
{


    void Debuger::SetBreakPoint(BpType type, Address_t adress)
    {
        switch (type)
        {
        case Debuger::simplePoint:
            SetSimpleBreakePoint(adress);
            break;
        case Debuger::readMem:
            SetBreakePointOnReadMemory(adress);
            break;
        case Debuger::WriteMem:
            SetBreakePointOnWriteMemory(adress);
            break;
        case Debuger::CallFunc:
            //
            break;
        default:
            break;
        }
    }

    void Debuger::RemoveBreakPoint(BpType type, Address_t adress)
    {
        switch (type)
        {
        case Debuger::simplePoint:
            SimpleBreakePoint.erase(adress);
            break;
        case Debuger::readMem:
            ReadMemory.erase(adress);
            break;
        case Debuger::WriteMem:
            WritePoint.erase(adress);
            break;
        case Debuger::CallFunc:
            //
            break;
        default:
            break;
        }
    }

    void Debuger::SetSimpleBreakePoint(Address_t adress)
    {
        Word_t saveMem = cpu.memory[adress]; // Сохраним то что лежит по адресу точки остановки
        SimpleBreakePoint[adress] = saveMem; // Создадим точку остановки
        Cmd16_t cmd; // Команда-замена для остановки процессора
        cmd.code = 0;
        cpu.memory[adress] = cmd.data;
    }

    void Debuger::SetBreakePointOnReadMemory(Address_t adress)
    {
        ReadMemory.insert(adress);
    }

    void Debuger::SetBreakePointOnWriteMemory(Address_t adress)
    {
        WritePoint[adress] = (Word_t)cpu.memory[adress];
    }

    void Debuger::RunWithoutStop()
    {
        cpu.psw.tf = 0;
        OperatingMode = Mode::DontStop;
        cpu.Run();
        while (cpu.GetCmd().cmd16.code != CPU::stop)
        {
            if (SimpleBreakePoint.find(cpu.psw.ip) != SimpleBreakePoint.end())
            {
                cpu.memory[cpu.psw.ip] = SimpleBreakePoint[cpu.psw.ip];
            }
            cpu.Run();
        }
    }

    void Debuger::RunToCheckpoint()
    {
        cpu.psw.tf = 1; // Установим флаг трассировки 
        OperatingMode = Mode::ToBP; // Режим дебагера 
        while (cpu.GetCmd().cmd16.code != CPU::stop)
        {
            int ip = cpu.psw.ip;  // Сохраним текущий IP
            cpu.Run(); // Запустим ВМ
            if (cpu.psw.ip - ip == 2) // Была команда c адресом 
            {
                Word_t calcAdress = cpu.memory[cpu.psw.ip - 1]; // Вычесленный адрес
                if (ReadMemory.find(calcAdress) != ReadMemory.end()) // Точка останова по чтению адреса
                {
                    std::cout << "BreakPoint ReadMem" << calcAdress << std::endl;
                    break;
                }
                if (WritePoint.find(calcAdress) != WritePoint.end()) //Точка останова по записи по адресу
                {
                    if (WritePoint[calcAdress] != cpu.memory[calcAdress]) // Если данные по адресу изменились 
                    {
                        std::cout << "BreakPoint WriteMem" << calcAdress << std::endl;
                        WritePoint[calcAdress] = cpu.memory[calcAdress];
                        break;
                    }
                }
            }

        }
        if (SimpleBreakePoint.find(cpu.psw.ip) != SimpleBreakePoint.end()) // Точка останова по достижению адреса
        {
            std::cout << "BreakPoint " << SimpleBreakePoint[cpu.psw.ip] << std::endl;
            cpu.memory[cpu.psw.ip] = SimpleBreakePoint[cpu.psw.ip]; // Сразу заменим данные
        }
    }

    void Debuger::RunStepByStep()
    {
        cpu.psw.tf = 1;
        OperatingMode = Mode::stepByStep;
        cpu.Run();
    }

    void Debuger::doInput(std::string str)
    {
        std::vector<std::string> parseLineCommand = ParseCommand(str);

        if (parseLineCommand[0] == "bp") // Установка точки останова
        {
            BreakPointAtion(std::vector<std::string>(parseLineCommand.begin() + 1, parseLineCommand.end()));
            return;
        }
        if (parseLineCommand[0] == "sw") // Просмотр состояния
        {
            if (parseLineCommand[1] == "r")
            {
                ShowRegister(parseLineCommand[2]); return;
            }
            if (parseLineCommand[1] == "m")
            {
                if (isNumber(parseLineCommand[3]) && isNumber(parseLineCommand[4]))
                {
                    ShowMemory(parseLineCommand[2], std::atoi(parseLineCommand[3].c_str()), std::atoi(parseLineCommand[4].c_str())); return;
                }
            }
        }
        if (parseLineCommand[0] == "set") // Установить значения регистр\память
        {
            if (parseLineCommand[1] == "r")
            {
                if (isNumber(parseLineCommand[3]))
                    SetRegister(parseLineCommand[2], std::atoi(parseLineCommand[3].c_str()), parseLineCommand[4]); return;
            }
            if (parseLineCommand[1] == "m")
            {
                if (isNumber(parseLineCommand[3]))
                    SetMemory(parseLineCommand[2], std::atoi(parseLineCommand[3].c_str()), parseLineCommand[4]); return;
            }
        }
        if (parseLineCommand[0] == "ex") // Выполнение в определенном режиме
        {
            if (parseLineCommand[1] == "bp") // до bp
            {
                RunToCheckpoint(); return;
            }
            if (parseLineCommand[1] == "sbs") // пошагово
            {
                RunStepByStep(); return;
            }
            if (parseLineCommand[1] == "ds") //без остановоко
            {
                RunWithoutStop(); return;
            }
        }
        std::cout << "Error param" << std::endl;
    }

    void Debuger::ShowRegister(std::string mode)
    {
        if (mode == "i")
        {
            for (size_t i = 0; i < 8; i++)
            {
                std::cout << "R" << i + 1 << " " << cpu.reg.Int[i].int32 << " " << cpu.reg.Int[i].uint32 << std::endl;
            }
        }
        else if (mode == "f")
        {
            for (size_t i = 0; i < 8; i++)
            {
                std::cout << "R" << i + 1 << " " << cpu.reg.Flt[i].flt32 << std::endl;
            }
        }
        else if (mode == "a")
        {
            for (size_t i = 0; i < 8; i++)
            {
                std::cout << "R" << i + 1 << " " << IntToHex(cpu.reg.Int[i].int32) << std::endl;
            }
        }
        else std::cout << "Error param" << std::endl;
    }

    void Debuger::ShowMemory(std::string mode, int16_t start, int16_t end)
    {
        if (mode == "i")
        {
            for (size_t i = start; i <= end; i++)
            {
                std::cout << "Mem[" << i + 1 << "] " << cpu.memory[i] << " " << (uint16_t)cpu.memory[i] << std::endl;
            }
            return;
        }
        if (mode == "f")
        {
            for (size_t i = start; i <= end; i++)
            {
                Flt32_t flt;
                flt.word[0] = cpu.memory[i];
                std::cout << "Mem[" << i + 1 << "] " << flt.flt32 << std::endl;
            }
            return;
        }
        if (mode == "b")
        {
            for (size_t i = start; i <= end; i++)
            {
                std::bitset<16> binForm(cpu.memory[i]);
                std::cout << "Mem[" << i + 1 << "] " << binForm << std::endl;
            }
            return;
        }
        if (mode == "c")
        {
            for (size_t i = start; i <= end; i++)
            {
                Cmd16_t cmd;
                cmd.data = cpu.memory[i];
                std::cout << "Mem[" << i + 1 << "] " << "c=" << cmd.code << " r1=" << cmd.r1 << " r2=" << cmd.r2 << " r3=" << cmd.r3 << std::endl;
            }
            return;
        }
    }

    Debuger::BpType Debuger::GetBpType(std::string ch)
    {
        if (ch == "b")
        {
            return BpType::simplePoint;
        }
        if (ch == "r")
        {
            return BpType::readMem;
        }
        if (ch == "w")
        {
            return  BpType::WriteMem;
        }
    }

    void Debuger::BreakPointAtion(std::vector<std::string> arg)
    {
        BpType breakPointType = GetBpType(arg[1]);
        if (arg[0] == "rem") // Удалить точку остановки
        {
            RemoveBreakPoint(breakPointType, std::atoi(arg[2].c_str())); return;
        }
        if (arg[0] == "allrem") // Удалить точки останова
        {
            SimpleBreakePoint.clear();
            ReadMemory.clear();
            WritePoint.clear();
            return;
        }
        if (arg[0] == "set")// Добавление точки
        {
            SetBreakPoint(breakPointType, std::atoi(arg[2].c_str())); return;
        }
        std::cout << "Error param" << std::endl;
    }

    void Debuger::SetRegister(std::string mode, int reg, std::string value)
    {
        if (mode == "i")
        {
            cpu.reg.Int[reg].int32 = std::atoi(value.c_str()); return;
        }
        if (mode == "f")
        {
            cpu.reg.Flt[reg].flt32 = std::atof(value.c_str()); return;
        }
        if (mode == "a")
        {
            std::stringstream ss;
            ss << std::hex << value;
            ss >> cpu.reg.Int[reg].int32;
            return;
        }
        std::cout << "Error param" << std::endl;

    }

    void Debuger::SetMemory(std::string mode, int adr, std::string value)
    {
        if (mode == "i")
        {
            cpu.memory[adr] = std::atoi(value.c_str()); return;
        }
        if (mode == "f")
        {
            Flt32_t flt;
            flt.flt32 = std::atof(value.c_str());
            cpu.memory[adr] = flt.word[0];
            return;
        }
        if (mode == "a")
        {
            std::stringstream ss;
            ss << std::hex << value;
            ss >> cpu.memory[adr];
            return;
        }
        std::cout << "Error param" << std::endl;
    }

    std::vector<std::string> Debuger::ParseCommand(std::string str)
    {
        std::vector<std::string> slit = split(str, ' '); // Разделяем строку по пробелам
        slit.erase(std::remove(slit.begin(), slit.end(), ""), slit.end()); // Удаляем нулевые элементы массива
        return slit;
    }

    void Debuger::Run(std::string fileName)
    {
        Loader loader;

        if (loader.LoadFromBinary(cpu, 0, fileName))
        {
            Console();
        }
        else
        {
            std::cout << "Error load" << std::endl;
        }
    }

    std::string Debuger::GetInput()
    {
        std::string input;
        std::cout << "> ";
        std::cin >> input;
        return input;
    }

    void Debuger::Console()
    {
        std::string input = GetInput();
        while (input != "exit")
        {
            doInput(input);
            input = GetInput();
        }
        std::cout << "Stop..." << std::endl;
    }

    void Debuger::Next()
    {
        if (OperatingMode == Mode::ToBP)
        {
            RunToCheckpoint(); return;
        }
        if (OperatingMode == Mode::stepByStep)
        {
            cpu.Run(); return;
        }
        std::cout << "Select debug mode" << std::endl;
    }
}
