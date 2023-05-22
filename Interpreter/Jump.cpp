#include "Jump.h"
namespace Interpretator
{



        void JumpDirect::go_to(CPU& cpu, int type) noexcept
        {
            const uint8_t r3 = cpu.GetCmd().cmd16.r3;
            switch (type)
            {
            case 1:
            {
                int16_t offset = cpu.GetCmd().cmd16.data;
                offset &= 0b11111111;
                cpu.psw.ip += offset;
                break;
            }
            case 2:
            {
                cpu.psw.ip = (cpu.reg.Int[r3].word[0] - 1);
                break;
            }
            case 3:
            {
                cpu.psw.ip = (cpu.GetCmd().adress - 2);
                break;
            }
            case 4:
            {
                cpu.psw.SetIP(cpu.reg.Int[r3].word[0] + (int16_t)cpu.GetCmd().adress);
                break;
            }
            default:
                break;
            }

        }

        int JumpDirect::getType(CPU& cpu) noexcept
        {
            const uint8_t r1 = cpu.GetCmd().cmd16.r1,
                r2 = cpu.GetCmd().cmd16.r2;
            if (r1 & 0b100)// относительный <r1/r2/r3>
            {
                return 1;
            }
            else if (r2 == 0/*(r1 & 0b100) == 0*/) // Косвенный
            {
                return 2;
            }
            else
            {
                // Прямой
                cpu.isLongCmd = true;
                if (r2 == 0b001)
                {
                    return 3;
                }
                else if (r2 == 0b011)
                {
                    return 4;
                }
            }
            return -1;
        }


        void JumpDirect::operator()(CPU& cpu) noexcept
        {
            int type = getType(cpu);
            go_to(cpu, type);
        }

        void je::operator()(CPU& cpu) noexcept
        {
            int type = getType(cpu);
            if (cpu.psw.GetZF() == 1)
                go_to(cpu, type);
        }

        void jne::operator()(CPU& cpu) noexcept
        {
            int type = getType(cpu);
            if (cpu.psw.GetZF() == 0)
                go_to(cpu, type);
        }

        void jl::operator()(CPU& cpu) noexcept
        {
            int type = getType(cpu);
            if (cpu.psw.GetSF() != cpu.psw.GetOF())
                go_to(cpu, type);
        }

        void jle::operator()(CPU& cpu) noexcept
        {
            int type = getType(cpu);
            if (cpu.psw.GetSF() != cpu.psw.GetOF() || cpu.psw.GetZF() == 1)
                go_to(cpu, type);
        }

        void jg::operator()(CPU& cpu) noexcept
        {
            int type = getType(cpu);
            if (cpu.psw.GetSF() == cpu.psw.GetOF() && cpu.psw.GetZF() == 0)
                go_to(cpu, type);
        }

        void jge::operator()(CPU& cpu) noexcept
        {
            int type = getType(cpu);
            if (cpu.psw.GetSF() == cpu.psw.GetOF())
                go_to(cpu, type);
        }

        void jb::operator()(CPU& cpu) noexcept
        {
            int type = getType(cpu);
            if (cpu.psw.GetCF() == 1)
                go_to(cpu, type);
        }

        void jbe::operator()(CPU& cpu) noexcept
        {
            int type = getType(cpu);
            if (cpu.psw.GetCF() == 1 || cpu.psw.GetZF() == 1)
                go_to(cpu, type);
        }

        void ja::operator()(CPU& cpu) noexcept
        {
            int type = getType(cpu);
            if (cpu.psw.GetCF() == 0 || cpu.psw.GetZF() == 0)
                go_to(cpu, type);
        }

        void jae::operator()(CPU& cpu) noexcept
        {
            int type = getType(cpu);
            if (cpu.psw.GetCF() == 0)
                go_to(cpu, type);
        }

        void call::operator()(CPU& cpu) noexcept
        {
            cpu.isLongCmd = true;
            cpu.reg.Int[7].uint32 = cpu.psw.ip;
            cpu.psw.ip = cpu.GetCmd().adress - 2;
        }

        void ret::operator()(CPU& cpu) noexcept
        {
            cpu.psw.ip = cpu.reg.Int[7].uint32;
        }
    }
