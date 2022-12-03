#pragma once
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

namespace proc
{
	/// <summary>
	/// Gets the ProcessID of the process with an exe matching procName.
	/// Returns 0 if none was found.
	/// </summary>
	/// <param name="procName"></param>
	/// <returns></returns>
	DWORD GetProcId(const wchar_t* procName);

	/// <summary>
	/// Gets the base address of modName within process of procId.
	/// Returns 0 if modName wasn't found.
	/// </summary>
	/// <param name="procId"></param>
	/// <param name="modName"></param>
	/// <returns></returns>
	uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);
}

