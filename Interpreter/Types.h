#pragma once
#include <cstdint>

using Address_t = uint16_t;
using Word_t = int16_t;

/*Команды 16 и 32 бит,результат в первом операнде*/
// Команда 16 бит -- КОП – 7 бит, r1 – 3 бита, r2 – 3 бита, r3 – 3 бита
union Cmd16_t
{
    struct {
        uint16_t code : 7;
        uint16_t r1 : 3;
        uint16_t r2 : 3;
        uint16_t r3 : 3;
    };
    uint16_t data;
};
// Команда 32 бит --КОП – 7 бит, r1 – 3 бита, r2 – 3 бита, r3 – 3 бита, адрес (константа) – 16 бит
struct Cmd32_t
{
	Cmd16_t cmd16;
	Address_t adress; // --адрес (константа)
};

using usual_cmd_t = Cmd32_t; // Для удобства

// Целые знаковые, беззнаковые
union Int32_t
{ 
	uint32_t uint32;
	int32_t int32;
	Word_t word[2];
};

// Дробные
union Flt32_t
{
	float flt32;
	Word_t word[2];
};

// Ячейка памяти под команду или адрес
union Memory_cmd
{
	Cmd16_t cmd;
	Address_t addres;
    Word_t word;
};

union Data32_t
{
    Int32_t int32;
    Flt32_t flt;
};

// Регистры по 32 бит : 8
union Register_t
{
	Int32_t Int[8];
	Flt32_t Flt[8];
};
/// PSW = IP + FLAG todo back to struct
class PSW
{
public:
	uint16_t ip { 0 };			  // Счетчик команд
	uint16_t zf : 1;             // Флаг нуля
	uint16_t sf : 1;             // Флаг знака
	uint16_t cf : 1;			  // Флаг переноса
	uint16_t of : 1;			  // Флаг переполнения
    uint16_t tf : 1;               // Флаг трассировки
	uint16_t reserv : 11;        // Резерв

	uint16_t GetIP() { return ip; }
	uint16_t GetOF() { return of; }
	uint16_t GetZF() { return zf; }
	uint16_t GetSF() { return sf; }
	uint16_t GetCF() { return cf; }
   
	void SetIP(Address_t ip) { ip = ip; }
	void AddIP(int add) { ip += add; }
	void SetZF(uint16_t zf) { zf = zf; }
	void SetSF(uint16_t sf) { sf = sf; }
	void SetCF(uint16_t cf) { cf = cf; }
	void SetOF(uint16_t of) { of = of; }

};
