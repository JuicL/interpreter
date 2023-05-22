#include "iMath.h"
namespace Interpretator
{
 
        void IntegerArithmetic::set_flags(CPU& cpu, int32_t var1, int32_t var2, uint64_t result) noexcept
        {
            cpu.psw.sf = (cpu.reg.Int[cpu.GetCmd().cmd16.r1].int32 < 0);
            cpu.psw.zf = ((result == 0) ? 1 : 0);
            cpu.psw.cf = ((result & 0x100000000) ? 1 : 0);
            cpu.psw.of = ((result > std::numeric_limits<uint32_t>::max()
                || result > std::numeric_limits<int32_t>::max()) ? 1 : 0); // ??

        }

        void IntegerArithmetic::operator()(CPU& cpu) noexcept
        {
            const int32_t reg2 = cpu.reg.Int[cpu.GetCmd().cmd16.r2].int32,
                reg3 = cpu.reg.Int[cpu.GetCmd().cmd16.r3].int32;

            uint64_t result;
            result = calculate(reg2, reg3);
            cpu.reg.Int[cpu.GetCmd().cmd16.r1].int32 = result;
            set_flags(cpu, reg2, reg3, result);
        }
    }
