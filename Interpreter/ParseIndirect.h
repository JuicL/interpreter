#pragma once
#include <utility>
#include <string>
#include <vector>
#include "ArifmeticParser.h"
class ParseIndirect
{
public:
    ParseIndirect()
    {
    }
    /*
    * return Пара: 1 -- Номер геристра, 2 -- Смещение прибавляемое к регистру(м.б. отриц.)
    */
    std::pair<int, int> ToParseInderect(std::string str)
    {
        std::pair<int,int> result;

        std::string deleteChars = "[] ";

        for (size_t i = 0; i < deleteChars.size(); ++i)
        {
            str.erase(std::remove(str.begin(), str.end(), deleteChars[i]), str.end());
           // str.erase(remove_if(str.begin(), str.end(), deleteChars[i]), str.end());
        }
        int posRegister = str.find('r');
        if (posRegister != std::string::npos)
        {
            int numReg = str[posRegister + 1] - '0';
            if (std::isdigit(str[posRegister + 1]) and numReg > 0 and numReg < 9)
            {
                result.first = numReg-1;
                str.erase(posRegister, 2); // r1,r2...r8
            }
            else
            {
                throw std::logic_error("Ошибка косвенной адресации");
            }
            if (str[0]== '+' || str[0] == '-') // Проверка на запись формата -- "r +(-) выражение".
            {
                ArifParser parser;
                result.second = parser.Result(str.c_str());
            }
            else
            {
                result.second = 0;
                //throw std::logic_error("Ошибка косвенной адресации");
            }
        }
        else
            throw std::logic_error("Ошибка косвенной адресации");
        return result;
    }

};

