#pragma once
#include "Command.h"
#include <cstdint>
namespace Interpretator
{
    
        /*!
            \brief Арифметика целых чисел

            \details Родительский класс для всех арифметический действий над целыми числами
        */
        class IntegerArithmetic : public Command
        {
            ///Установка флагов
            static void set_flags(CPU& cpu, int32_t var1, int32_t var2, uint64_t result) noexcept;
        protected:
            /*!
                Чисто виртуальная функия для вычисления результата арифм. действий

                \details Каждый дочерник класс реализует свое поведение метода calculate
                \param[in] var1,var2 числа над которыми производится операция
                \return Результат в соответсвии с действием реализованным в дочернем классе
            */
            virtual int calculate(int32_t var1, int32_t var2) noexcept = 0;
        public:
            /*!
                Функтор наследуемый всеми дочерними классами.

                \details Реализует арифметические действия над целыми числами
            */
            void operator()(CPU& cpu) noexcept override;
        };
        /// Сложение целых чисел
        class iAdd : public IntegerArithmetic
        {
            /*!
               Вычисляет сумму двух чисел

               \param[in] var1,var2 Складываемые числа
               \return Сумму двух чисел,переданных в качестве аргументов
            */
            int calculate(int32_t var1, int32_t var2) noexcept final
            {
                return var1 + var2;
            }
        };
        /// Разность целых чисел
        class iSub : public IntegerArithmetic
        {
            /*!
               Вычисляет разность двух чисел

               \param[in] var1  Уменьшаемое
               \param[in] var2  Вычитаемое
               \return Разность двух чисел,переданных в качестве аргументов
            */
            int calculate(int32_t var1, int32_t var2) noexcept final
            {
                return var1 - var2;
            }
        };
        /// Произведение целых чисел
        class iMul : public IntegerArithmetic
        {
            /*!
                Вычисляет произведние двух чисел

                \param[in] var1,var2  Множители
                \return Произведние двух чисел,переданных в качестве аргументов
            */
            int calculate(int32_t var1, int32_t var2) noexcept final
            {
                return var1 * var2;
            }
        };
        /// Частное целых чисел
        class iDiv : public IntegerArithmetic
        {
            /*!
                Вычисляет частное двух чисел

                \param[in] var1  Делимое
                \param[in] var2  Делитель
                \return Частное двух чисел,переданных в качестве аргументов
            */
            int calculate(int32_t var1, int32_t var2) noexcept final
            {
                return var1 / var2;
            }
        };
        //=======================
        // Битовые операции
        //=======================
        /// Побитовое И
        class bitAnd : public IntegerArithmetic
        {
            /*!
                Вычисляет побитовое И
                \return Побитовое И(&) двух чисел,переданных в качестве аргументов
            */
            int calculate(int32_t var1, int32_t var2) noexcept final
            {
                return var1 & var2;
            }
        };
        /// Побитовая инверсия
        class bitNot : public IntegerArithmetic
        {
            /*!
                Вычисляет побитовую инверсию
                \return Побитовая инверсия первого аргумента var1
            */
            int calculate(int32_t var1, int32_t var2) noexcept final
            {
                return ~var1;
            }
        };
        /// Побитовое ИЛИ
        class bitOr : public IntegerArithmetic
        {
            /*!
                Вычисляет побитовое ИЛИ
                \return Побитовое ИЛИ(|) двух чисел,переданных в качестве аргументов
            */
            int calculate(int32_t var1, int32_t var2) noexcept final
            {
                return var1 | var2;
            }
        };
        /// Исключающее ИЛИ
        class bitXor : public IntegerArithmetic
        {
            /*!
                Вычисляет побитовое исключающее ИЛИ
                \return исключающее ИЛИ(^) двух чисел,переданных в качестве аргументов
            */
            int calculate(int32_t var1, int32_t var2) noexcept final
            {
                return var1 ^ var2;
            }
        };
        /// Побитовый сдвиг влево
        class shifL : public IntegerArithmetic
        {
            /*!
               Побитовый сдвиг влево
               \param[in] var1 Число над которым производится сдвиг
               \param[in] var2 Число равное количеству бит,на которое надо совершить сдвиг
               \return var1 сдвинутое влево на var2 бит
            */
            int calculate(int32_t var1, int32_t var2) noexcept final
            {
                return (var1 << var2);
            }
        };
        /// Побитовый сдвиг вправо
        class shifR : public IntegerArithmetic
        {
            /*!
               Побитовый сдвиг вправо
               \param[in] var1 Число над которым производится сдвиг
               \param[in] var2 Число равное количеству бит,на которое надо совершить сдвиг
               \return var1 сдвинутое вправо на var2 бит
            */
            int calculate(int32_t var1, int32_t var2) noexcept final
            {
                return (var1 >> var2);
            }
        };
}
