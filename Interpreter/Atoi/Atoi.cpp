#include "Atoi.h"

bool Atoi::isDec(std::string Number)
{
	States state = start;
	for (size_t i = 0; i < Number.size() && state != err; i++)
	{
		char ch = Number[i];
		switch (state)
		{
		case States::start:
		{
			if (ch == '+' || ch == '-')
				state = s1;
			else if (isdigit(ch))
				state = s2;
			else
				state = err;
			break;
		}
		case States::s1:
		{
			if (isdigit(ch))
				state = s2;
			else state = err;
			break;
		}
		case States::s2:
		{
			if (isdigit(ch))
				state = s2;
			else state = err;
			break;
		}
		}
	}
	if (state == s2) return true;
    return false;
}

bool Atoi::isHex(std::string Number)
{
	States state = start;
	for (size_t i = 0; i < Number.size(); i++)
	{
		char ch = Number[i];
		switch (state)
		{
		case start:
			if (ch == '0')
				state = s1;
			else
				state = err;
			break;
		case s1:
			if (ch == 'x')
				state = s2;
			else state = err;
			break;
		case s2:
			if (ch >= '0' && ch <= '9' || ch >= 'A' && ch <= 'F')
				state = s3;
			else state = err;
			break;
        case s3:
            if (ch >= '0' && ch <= '9' || ch >= 'A' && ch <= 'F')
                state = s3;
            else state = err;
            break;
		case err:
			return false;
			break;
		}
	}
    if (state == s3) return true;
	return false;
}

bool Atoi::isBin(std::string Number)
{
	States state = start;
	for (size_t i = 0; i < Number.size(); i++)
	{
		char ch = Number[i];
		switch (state)
		{
		case start:
			if (ch == '0')
				state = s1;
			else
				state = err;
			break;
		case s1:
			if (ch == 'b')
				state = s2;
			else state = err;
			break;
		case s2:
			if (ch == '0' || ch == '1')
				state = s3;
			else state = err;
			break;
        case s3:
            if (ch == '0' || ch == '1')
                state = s3;
            else state = err;
            break;
		case err:
			return false;
			break;
		}
	}
    if (state == s3) return true;
    return false;
}

int Atoi::toDec(std::string Number)
{
	int res = 0;
	bool isNegative = false;
	for (size_t i = 0; i < Number.size(); i++)
	{
		char ch = Number[i];
		if (ch == '-')
			isNegative = true;
		else if (isdigit(ch))
		{
			res *= 10;
			res += ch - '0';
		}
	}
	if (isNegative)
		res *= -1;
	return res;
}

int Atoi::toHex(std::string Number)
{
	int res = 0;
	std::stringstream ss;
	ss << std::hex << Number.substr(2);
	ss >> res;
	return res;

}

int Atoi::toBin(std::string Number)
{
	int res = 0;
	int degree = 2;
	if (Number[Number.size() - 1] == '1')
		res += 1;
	for (size_t i = Number.size() - 2; i > 1; i--)
	{
		if (Number[i] == '1')
			res += degree;
		degree *= 2;
	}
	return res;

}

bool Atoi::ParseNumber(std::string str,int &res)
{
	if (isDec(str))
	{
		res = toDec(str);
		return true;
	}
	if (isHex(str))
	{
		res = toHex(str);
		return true;
	}
	if (isBin(str))
	{
		res = toBin(str);
		return true;
	}
	return false;
}
