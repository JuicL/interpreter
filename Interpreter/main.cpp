#include <iostream>
#include <algorithm>
#include "CPU.h"
#include "Loader/Loader.h"
#include "Translator/Translator.h"


std::string GetPathToDirectory(std::string fullPath)
{
    int pos = fullPath.rfind('\\');
    return fullPath.substr(0, pos+1);
}

int main(int argc, char* argv[])
{
    std::string pathToDirectory = GetPathToDirectory(argv[0]);

    
    Translator::Translator t(pathToDirectory);
    try
    {
        t.Translate(pathToDirectory + "test4.txt");
        Loader ld;
        Interpretator::CPU cpu;
        ld.LoadFromBinary(cpu, 0, pathToDirectory + "testbin.bin");
        cpu.Run();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    
    system("pause");
	return 0;
}
