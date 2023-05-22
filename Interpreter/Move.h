#pragma once
#include "Command.h"
namespace Interpretator
{
    
        /// Пересылка данных
        class move : public Command
        {
        public:
            /*!
                Функтор реализующий операции пересылки данных

                \details Значение второго регистра указанного в команде хранит тип пересылки
            */
            void operator()(CPU& cpu) noexcept override;
        };
}
