#pragma once
#include <cinttypes>
#include <iostream>
#include <limits>
#include <cmath>
#include "CPU.h"
namespace Interpretator
{
 
    /*!
        \brief Базовый класс команд
    */
    class Command
    {
    public:
        Command() = default;
        virtual void operator()(CPU& processor) noexcept = 0;
        virtual ~Command() = default;
    };
}
