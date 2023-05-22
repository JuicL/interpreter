#pragma once
#include "Command.h"
namespace Interpretator
{
   
   
        class Input : public Command
        {
        public:
            /*!
            *
            */
            void operator()(CPU& cpu) noexcept override;
        };

        class Output : public Command
        {
        public:
            /*!
            *
            */
            void operator()(CPU& cpu) noexcept override;
        };
}
