#include "Extention.h"

std::vector<std::string> split(const std::string& s, char symb)

{
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, symb))
	{
		elems.push_back(item);
	}
	return elems;
}

bool isNumber(std::string str)
{
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] > '9' || str[i] < '0')
            return false;
    }
    return true;
}
