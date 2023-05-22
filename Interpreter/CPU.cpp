#include "CPU.h"
#include "Command.h"
#include "Move.h"
#include "iMath.h"
#include "fMath.h"
#include "Jump.h"
#include "IO.h"
#include "Compare.h"
namespace Interpretator
{
    CPU::CPU()
    {
        _command.resize(CPU::end); // Выделяем память под классы операций
        isLongCmd = 0;
        _command[stop] = nullptr;
        _command[move] = std::make_unique<class move>();

        _command[iAdd] = std::make_unique<class iAdd>();
        _command[iSub] = std::make_unique<class iSub>();
        _command[iMul] = std::make_unique<class iMul>();
        _command[iDiv] = std::make_unique<class iDiv>();

        _command[fAdd] = std::make_unique<class fAdd>();
        _command[fSub] = std::make_unique<class fSub>();
        _command[fMul] = std::make_unique<class fMul>();
        _command[fDiv] = std::make_unique<class fDiv>();

        _command[And] = std::make_unique<class bitAnd>();
        _command[Or] = std::make_unique<class bitOr>();
        _command[Not] = std::make_unique<class bitNot>();
        _command[Xor] = std::make_unique<class bitXor>();

        _command[shif_L] = std::make_unique<class shifL>();
        _command[shif_R] = std::make_unique<class shifR>();

        _command[in] = std::make_unique<class Input>();
        _command[out] = std::make_unique<class Output>();

        _command[Icmp] = std::make_unique<class Compare>();
        _command[Fcmp] = std::make_unique<class CompareF>();

        _command[jmp] = std::make_unique<class JumpDirect>();
        _command[je] = std::make_unique<class je>();
        _command[jne] = std::make_unique<class jne>();
        _command[jl] = std::make_unique<class jl>();
        _command[jle] = std::make_unique<class jle>();
        _command[jg] = std::make_unique<class jg>();
        _command[jge] = std::make_unique<class jge>();
        _command[jb] = std::make_unique<class jb>();
        _command[jbe] = std::make_unique<class jbe>();
        _command[ja] = std::make_unique<class ja>();
        _command[jae] = std::make_unique<class jae>();

        _command[call] = std::make_unique<class call>();
        _command[ret] = std::make_unique<class ret>();


    }

    CPU::~CPU()
    {
    }

    void CPU::Reset() noexcept
    {
        psw.SetCF(0);
        psw.SetOF(0);
        psw.SetSF(0);
        psw.SetZF(0);
        for (uint8_t i = 0; i < 8; i++)
        {
            reg.Int[i].int32 = 0;
        }
    }

    void CPU::Run() noexcept
    {
        LoadCurCommand();

        while (_cmd.cmd16.code != stop)
        {
            _command[_cmd.cmd16.code]->operator()(*this);
            if (psw.tf)
                break;
            psw.AddIP(1);
            if (isLongCmd) // Если команда занимает 2 слова памяти ( Бит устанавливается внутри команды )
            {
                psw.AddIP(1);
                isLongCmd = 0;
            }
            LoadCurCommand();
        }
    }

    void CPU::LoadCurCommand() noexcept
    {
        _cmd.cmd16.data = memory[psw.GetIP()];
        _cmd.adress = memory[psw.GetIP() + 1];
    }
}
