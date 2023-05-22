#include "IO.h"
#include <string>
namespace Interpretator
{

        /*!
            \brief Операции ввода

            2 Режима ввода: в регистр,в память(строку)
        */

        void Input::operator()(CPU& cpu) noexcept
        {
            const uint8_t r1 = cpu.GetCmd().cmd16.r1,
                dataType = cpu.GetCmd().cmd16.r2;
            switch (dataType)
            {
            case CPU::ioInt:
            {
                int32_t input;
                std::cin >> input;
                cpu.reg.Int[r1].int32 = input;
                break;
            }
            case CPU::ioUint:
            {
                uint32_t input;
                std::cin >> input;
                cpu.reg.Int[r1].uint32 = input;
                break;
            }
            case CPU::ioFloat:
            {
                float input;
                std::cin >> input;
                cpu.reg.Flt[r1].flt32 = input;
                break;
            }
            }
        }

        void Output::operator()(CPU& cpu) noexcept
        {
            const uint8_t r1 = cpu.GetCmd().cmd16.r1,
                dataType = cpu.GetCmd().cmd16.r2;
            switch (dataType)
            {
            case CPU::ioInt:
            {
                std::cout << cpu.reg.Int[r1].int32 << std::endl;
                break;
            }
            case CPU::ioUint:
            {
                std::cout << cpu.reg.Int[r1].uint32 << std::endl;
                break;
            }
            case CPU::ioFloat:
            {
                std::cout << cpu.reg.Flt[r1].flt32 << std::endl;
                break;
            }
            }
        }
    }
