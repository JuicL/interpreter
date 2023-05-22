#include "fMath.h"
namespace Interpretator
{

        void FloatArithmetic::set_flags(CPU& cpu, float result) noexcept
        {
            constexpr float eps = std::numeric_limits<float>::epsilon();
            cpu.psw.SetZF((result <= eps) ? 1 : 0);
            cpu.psw.SetSF((result < 0) ? 1 : 0);
            cpu.psw.SetOF(0); // todo!
            cpu.psw.SetCF(0);
        }

        void FloatArithmetic::operator()(CPU& cpu) noexcept
        {
            const uint8_t r1 = cpu.GetCmd().cmd16.r1,
                r2 = cpu.GetCmd().cmd16.r2,
                r3 = cpu.GetCmd().cmd16.r3;
            cpu.reg.Flt[r1].flt32 = calculate(cpu.reg.Flt[r2].flt32, cpu.reg.Flt[r3].flt32);
            set_flags(cpu, cpu.reg.Flt[r1].flt32);
        }
}

