#pragma once
enum TypeCommand
{
    Move,
    Lea,
    Math,
    Io,
    Cmp,
    Jumps,
    Call,
    WithoutParam

};
enum TypeOrerand
{
    Register,
    Variable,
    Indirect,
    Const,
    Special,
    None
};
//todo ТЕПЕРЬ МаССИВ
struct ElemNameTable // Элемент таблицы имен
{
    uint8_t type; // Тип перепенной( если 0,то метка)
    size_t address; // Адресс переменной\метки
    std::vector<Data32_t> value; // Первоначальное значение,если не указано то 0
};
