// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <time.h>
#include "mem.h"
#include "game.h"

DWORD WINAPI HackThread(HMODULE hModule)
{
    // Create console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    uintptr_t moduleBase = (uintptr_t)GetModuleHandle((LPCWSTR)GAME_NAME);
    // Calling it with NULL also gives you the address of the .exe module
    moduleBase = (uintptr_t)GetModuleHandle(NULL);

    time_t lastUpdated = time(0);

    while (true)
    {
        // Write menu
        if (difftime(time(0), lastUpdated) >= 1)
        {
            system("cls");
            std::cout << "[F1] Test" << std::endl;
            std::cout << "[End] Exit" << std::endl << std::endl;

            std::vector<std::vector<BYTE>> grid = game::GetGrid();
            game::DisplayGrid(grid);

            time(&lastUpdated);
        }

        // Quit
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        // Test
        if (GetAsyncKeyState(VK_F1) & 1)
        {
            game::SaySomething("Hello, World!");
        }

        Sleep(5);
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

