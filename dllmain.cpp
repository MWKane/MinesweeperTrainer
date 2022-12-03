// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
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

    // Toggles
    bool displayGrid = false;

    bool updateMenu = true;
    while (true)
    {
        if (updateMenu)
        {
            system("cls");
            std::cout << "[F1] Option 1" << std::endl;
            std::cout << "[F2] Option 2" << std::endl;
            std::cout << "[F3] Option 3" << std::endl;

            std::cout << "[End] Exit" << std::endl << std::endl;

            updateMenu = false;
        }

        // Quit
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        // Option 1
        if (GetAsyncKeyState(VK_F1) & 1 || displayGrid)
        {
            displayGrid = !displayGrid;
            updateMenu = !displayGrid;

            std::vector<std::vector<BYTE>> grid = game::GetGrid();
            game::DisplayGrid(grid);
        }

        // Option 2
        if (GetAsyncKeyState(VK_F2) & 1)
        {
            // game::doSomething()
            updateMenu = true;
        }

        // Option 3
        if (GetAsyncKeyState(VK_F3) & 1)
        {
            // game::doSomething()
            updateMenu = true;
        }

        // ...
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

