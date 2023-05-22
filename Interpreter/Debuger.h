#pragma once
#include <unordered_map>
#include <unordered_set>
#include "CPU.h"
#include "Loader/Loader.h"
#include "Extention.h"
/* ФОРМАТ КОМАНД
    bp -- Команды точек останова
    Типы
        b - точка останова по достижению адреса
        r - по чтению адреса
        w - по записи по адресу
    bp rem <тип> <адрес> -- удалить точку остановки, тип -- как в установке
    bp allrem  -- Удалить все точки останова
    bp set <тип> <адрес> -- установить точку останова указанного типа

    sw -- Показать
    sw r <тип>-- Показать значения всех регистров
        типы:    i - как целое
                 f - как дробное
                 a - как адрес( hex)
    sw m <тип> left rigt -- Показать значения памяти в диапазоне left до right
        типы:    i - как целое
                 f - как дробное
                 b - двоичный вид
                 с - команда
    set r <тип> <значение> -- Установить значением регистр
        Тип такой же как и в команде просмотра(sw)
    set m <тип> <адрес> <значение> -- Установить значение памяти по адресу
        Тип такой же как и в команде просмотра(sw)

   ex -- Начать отладку
   ex bp -- Отладка до первой точки останова
   ex sbs -- Пошаговая отладка
   ex ds -- Отладка с игнорирированием точек останова
*/
namespace Debuger
{
    class Debuger
    {
        enum Mode // Режим работы 
        {
            ToBP, stepByStep, DontStop, None
        };
        enum BpType // Тип точки останова
        {
            simplePoint, readMem, WriteMem, CallFunc,
        };
        std::unordered_map<Address_t, Word_t> SimpleBreakePoint; // Точки останова по достижению адреса
        std::unordered_set<Address_t> ReadMemory; // Точки останова при чтении адреса
        std::unordered_map<Address_t, Word_t> WritePoint; // Точки останова при записи по адресу
        Mode OperatingMode; // Режим выполнения
        CPU cpu; // Процессор
        BpType GetBpType(std::string ch);
        // Установить точку останова
        void SetBreakPoint(BpType type, Address_t adress);
        // Удалить точку останова
        void RemoveBreakPoint(BpType type, Address_t adress);
        // Установить точку останова по достижению адреса
        void SetSimpleBreakePoint(Address_t adress);
        // Установить точку по заиписи
        void SetBreakePointOnReadMemory(Address_t adress);
        // Установить точку по чтению адреса
        void SetBreakePointOnWriteMemory(Address_t adress);
        // Запуск без точек останова
        void RunWithoutStop();
        // Запуск до первой след. точки остановки
        void RunToCheckpoint();
        // Пошагово
        void RunStepByStep();
        // Выполниить команду
        void doInput(std::string command);
        // Вывести регистры
        void ShowRegister(std::string mode);
        // Вывести память
        void ShowMemory(std::string mode, int16_t startRange, int16_t endRange);
        // Действия с точками останова
        void BreakPointAtion(std::vector<std::string> arg);
        // Установить значение регистра
        void SetRegister(std::string mode, int reg, std::string value);
        // Установить значение памяти
        void SetMemory(std::string mode, int adr, std::string value);
        // Разбить команду на токены
        std::vector<std::string> ParseCommand(std::string str);
        // Получить ввод из консоли
        std::string GetInput();
        // Главный цикл консоли
        void Console();
        // След шаг при отладке
        void Next();
    public:
        Debuger()
        {
            OperatingMode = None;
        }
        // Запуск дебагера
        void Run(std::string fileName);
    };

}
