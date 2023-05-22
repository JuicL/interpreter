#pragma once
#include "Command.h"
#include "iMath.h"
#include "fMath.h"
namespace Interpretator
{
    
        /// Сравнение целых чисел
        class Compare : public iSub
        {
            void operator ()(CPU& cpu) noexcept override;
        };

        /// Сравнение дробных чисел
        class CompareF : public fSub
        {
            void operator()(CPU& cpu) noexcept override;
        };
}

