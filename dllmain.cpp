// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <time.h>
#include "game.h"

DWORD WINAPI HackThread(HMODULE hModule)
{
    // Create console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    // Toggles
    bool isClockFrozen = false;

    time_t lastUpdated = time(0);
    while (true)
    {
        // Write menu
        if (difftime(time(0), lastUpdated) >= 1)
        {
            // Do any intense work before we cls the screen
            std::vector<std::vector<BYTE>> grid = game::GetGrid();

            system("cls");
            std::cout << "[F1] Toggle Clock" << std::endl;
            std::cout << "[F2] Reset Clock" << std::endl;
            std::cout << "[F3] Flag Mines" << std::endl;
            std::cout << "[F4] Win Game" << std::endl;
            std::cout << "[End] Exit" << std::endl << std::endl;

            game::DisplayGrid(grid);

            time(&lastUpdated);
        }

        // Quit
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        // Freeze Clock
        if (GetAsyncKeyState(VK_F1) & 1)
        {
            if (isClockFrozen)
            {
                game::UnfreezeClock();
            }
            else
            {
                game::FreezeClock();
            }

            isClockFrozen = !isClockFrozen;
        }

        // Reset Clock
        if (GetAsyncKeyState(VK_F2) & 1)
        {
            game::SetClock(0);
        }

        // Flag mines
        if (GetAsyncKeyState(VK_F3) & 1)
        {
            game::RevealMines();
        }

        // Win Game 
        if (GetAsyncKeyState(VK_F4) & 1)
        {
            game::WinGame();
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

