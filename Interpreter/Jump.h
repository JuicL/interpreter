#pragma once
#include "Command.h"
namespace Interpretator
{

    /*!
        \brief Прямой переход

        Родительский класс для остальных прыжков
    */
    class JumpDirect : public Command
    {
    protected:
        void go_to(CPU& cpu, int type) noexcept;
        int getType(CPU& cpu) noexcept;
    public:
        void operator()(CPU& cpu) noexcept override;
    };
    //-- Для всех чисел

    /*!
        \brief Переход если равно

        Условие перехода после команды CMP: op1 = op2 \n
        Значение флагов : ZF = 1
    */
    class je : public JumpDirect
    {
    public:
        void operator() (CPU& processor) noexcept override;
    };

    /*!
        \brief Переход если не равно

        Условие перехода после команды CMP: op1 != op2 \n
        Значение флагов : ZF = 0
    */
    class jne : public JumpDirect
    {
    public:
        void operator() (CPU& processor) noexcept override;
    };

    //Для чисел со знаком

    /*!
        \brief Переход если меньше

        Условие перехода после команды CMP: op1 < op2\n
        Значение флагов : SF != OF
    */
    class jl : public JumpDirect
    {
    public:
        void operator() (CPU& processor) noexcept override;
    };

    /*!
        \brief Переход если меньше или равно

        Условие перехода после команды CMP: op1 <= op2\n
        Значение флагов : SF != OF или ZF = 1
    */
    class jle : public JumpDirect
    {
    public:
        void operator() (CPU& processor) noexcept override;
    };

    /*!
        \brief Переход если больше

        Условие перехода после команды CMP: op1 > op2\n
        Значение флагов : SF = OF и ZF = 0
    */
    class jg : public JumpDirect
    {
    public:
        void operator() (CPU& processor) noexcept override;
    };

    /*!
        \brief Переход если больше или равно

        Условие перехода после команды CMP: op1 >= op2\n
        Значение флагов : SF = OF
    */
    class jge : public JumpDirect
    {
    public:
        void operator() (CPU& processor) noexcept override;
    };

    //--Для чисел без знака

    /*!
        \brief Переход если ниже

        Условие перехода после команды CMP: op1 < op2\n
        Значение флагов : CF = 1
    */
    class jb : public JumpDirect
    {
    public:
        void operator() (CPU& processor) noexcept override;
    };

    /*!
        \brief Переход если ниже или равно

        Условие перехода после команды CMP: op1 <= op2\n
        Значение флагов : CF = 1 или ZF = 1
    */
    class jbe : public JumpDirect
    {
    public:
        void operator() (CPU& processor) noexcept override;
    };

    /*!
        \brief Переход если выше

        Условие перехода после команды CMP: op1 > op2\n
        Значение флагов : CF = 0 и ZF = 0
    */
    class ja : public JumpDirect
    {
    public:
        void operator() (CPU& processor) noexcept override;
    };

    /*!
        \brief Переход если выше или равно

        Условие перехода после команды CMP: op1 >= op2\n
        Значение флагов : CF = 0
    */
    class jae : public JumpDirect
    {
    public:
        void operator() (CPU& processor) noexcept override;
    };

    /*!
        \brief Вызов процедуры

        Адрес возврата запоминается в 1 регистре
    */
    class call : public JumpDirect
    {
    public:
        void operator() (CPU& processor) noexcept override;
    };

    /*!
        \brief Возврат из процедуры
    */
    class ret : public JumpDirect
    {
    public:
        void operator() (CPU& processor) noexcept override;
    };
}
