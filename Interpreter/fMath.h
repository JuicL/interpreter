#pragma once
#include "Command.h"          
namespace Interpretator
{
   
        /*!
            \brief Арифметика дробных чисел

            \details Родительский класс для всех арифметический действий над целыми дробными числами
        */
        class FloatArithmetic : public Command
        {
            /// Установка флагов
            static void set_flags(CPU& cpu, float result) noexcept;
        protected:

            virtual float calculate(float var1, float var2) noexcept = 0;
        public:
            /*!
                Функтор наследуемый всеми дочерними классами.
                \details Реализует арифметические действия над дробными числами
            */
            void operator()(CPU& cpu) noexcept override;
        };
        /// Сложение дробных чисел
        class fAdd : public FloatArithmetic
        {
            /*!
              \brief Вычисляет сумму двух чисел

              \param[in] var1,var2 Складываемые числа
              \return Сумму двух чисел,переданных в качестве аргументов
           */
            float calculate(float var1, float var2) noexcept final
            {
                return var1 + var2;
            }
        };
        /// Разность дробных чисел
        class fSub : public FloatArithmetic
        {
            /*!
                \brief Вычисляет разность двух чисел

                \param[in] var1  Уменьшаемое
                \param[in] var2  Вычитаемое
                \return Разность двух чисел,переданных в качестве аргументов
            */
            float calculate(float var1, float var2) noexcept final
            {
                return var1 - var2;
            }
        };
        /// Произведение дробных чисел
        class fMul : public FloatArithmetic
        {
            /*!
                \brief Вычисляет произведние двух чисел

                \param[in] var1,var2  Множители
                \return Произведние двух чисел,переданных в качестве аргументов
            */
            float calculate(float var1, float var2) noexcept final
            {
                return var1 * var2;
            }
        };
        /// Частное дробных чисел
        class fDiv : public FloatArithmetic
        {
            /*!
                \brief Вычисляет частное двух чисел

                \param[in] var1  Делимое
                \param[in] var2  Делитель
                \return Частное двух чисел,переданных в качестве аргументов
            */
            float calculate(float var1, float var2) noexcept final
            {
                return var1 / var2;
            }
        };
    }
