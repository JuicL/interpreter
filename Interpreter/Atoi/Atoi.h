#pragma once
#include <string>
#include <sstream>
class Atoi
{
	enum States {
		start, s1, s2,s3, err
	};
	 bool isDec(std::string Number);
	 bool isHex(std::string Number);
     bool isBin(std::string Number);
	 int toDec(std::string Number);
	 int toHex(std::string Number);
	 int toBin(std::string Number);
public:
	Atoi()
	{}
	bool ParseNumber(std::string,int &res);

};

