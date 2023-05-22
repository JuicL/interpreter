#include "Compare.h"
namespace Interpretator
{

    void Compare::operator()(CPU& cpu) noexcept
    {
        Int32_t temp = cpu.reg.Int[cpu.GetCmd().cmd16.r1];
        iSub sub;
        sub.operator()(cpu);
        cpu.reg.Int[cpu.GetCmd().cmd16.r1] = temp;
    }

    void CompareF::operator()(CPU& cpu) noexcept
    {
        Flt32_t temp = cpu.reg.Flt[cpu.GetCmd().cmd16.r1];
        fSub sub;
        sub.operator()(cpu);
        cpu.reg.Flt[cpu.GetCmd().cmd16.r1] = temp;
    }
}
