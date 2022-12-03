#include "pch.h"
#include "proc.h"

DWORD proc::GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;

	// Create a snapshot of the target process
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		// Get the next process in the snapshot
		while (Process32Next(hSnap, &procEntry))
		{
			// Check if the process .exe is the same as procName
			if (!_wcsicmp(procEntry.szExeFile, procName))
			{
				procId = procEntry.th32ProcessID;
				break;
			}
		}
	}

	CloseHandle(hSnap);
	return procId;
}

uintptr_t proc::GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t addr = 0;
	
	// Create a snapshot of the target process
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		// Check if the process module name is the same as modName
		while (Module32Next(hSnap, &modEntry))
		{
			if (!_wcsicmp(modEntry.szModule, modName))
			{
				addr = (uintptr_t)modEntry.modBaseAddr;
				break;
			}
		}
	}

	CloseHandle(hSnap);
	return addr;
}
