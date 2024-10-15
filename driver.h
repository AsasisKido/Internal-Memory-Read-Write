#pragma once

#include <Windows.h>
#include <string>

enum Analysis
{
    Easy,
    Normal,
    Deep
};

class EnfusionProcess
{
public:
    static void AutoGameSelection(Analysis methods);
    static uint64_t ModuleBase;
    static const char* GameName;

    template <typename T>
    static T ReadData(uint64_t address)
    {
        if (!address)
            return T();

        T buffer;
        return ReadProcessMemory(GetCurrentProcess(), (LPCVOID)address, &buffer, sizeof(T), nullptr) ? buffer : T();
    };


    template <typename T>
    static void WriteData(uint64_t address, T data)
    {
        if (!address)
            return;

        WriteProcessMemory(GetCurrentProcess(), (LPVOID)address, &data, sizeof(T), nullptr);
    };

    static std::string ReadString(uint64_t address, size_t size)
    {
        if (!address || size > 1024)
            return "";

        char string[1024] = "";
        return ReadProcessMemory(GetCurrentProcess(), (LPCVOID)address, string, size, nullptr) ? std::string(string) : "";
    };

    static bool WriteString(uint64_t address, char* string, size_t size)
    {
        if (!address)
            return false;

        return WriteProcessMemory(GetCurrentProcess(), (LPVOID)address, string, size, nullptr) ? true : false;
    };

private:
    static uint64_t GetModuleBase()
    {
        HMODULE module = GetModuleHandle(GameName);
        if (module)
        {
            return (uint64_t)module;
        }
        return 0;
    }
};

const char* EnfusionProcess::GameName = "YourProcess.exe";
uint64_t EnfusionProcess::ModuleBase = 0;

void EnfusionProcess::AutoGameSelection(Analysis methods)
{
    if (methods == Analysis::Easy || methods == Analysis::Normal || methods == Analysis::Deep)
    {
        ModuleBase = GetModuleBase();
    }
}
