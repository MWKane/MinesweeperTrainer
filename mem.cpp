#include "pch.h"
#include "mem.h"

void mem::Patch(BYTE* dst, BYTE* src, unsigned int size)
{
	// Unprotect memory
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

	// Write memory
	memcpy(dst, src, size);

	// Protect memory
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess)
{
	// Unprotect memory
	DWORD oldprotect;
	VirtualProtectEx(hProcess, dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

	// Write memory
	WriteProcessMemory(hProcess, dst, src, size, nullptr);

	// Protect memory
	VirtualProtectEx(hProcess, dst, size, oldprotect, &oldprotect);
}

void mem::Nop(BYTE* dst, unsigned int size)
{
	// Unprotect memory
	DWORD oldprotect;
	VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldprotect);

	// Write NOP to memory
	memset(dst, NOP, size);

	// Protect memory
	VirtualProtect(dst, size, oldprotect, &oldprotect);
}

void mem::NopEx(BYTE* dst, unsigned int size, HANDLE hProcess)
{
	// Store NOP array in process memory
	BYTE* nopArray = new BYTE[size];
	memset(nopArray, NOP, size);

	// Copy NOP array to target process
	PatchEx(dst, nopArray, size, hProcess);

	// Remove NOP array from process memory
	delete[] nopArray;
}

uintptr_t mem::FindMultilevelPointer(uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		addr = *(uintptr_t*)addr;
		addr += offsets[i];
	}

	return addr;
}

uintptr_t mem::FindMultilevelPointerEx(uintptr_t ptr, std::vector<unsigned int> offsets, HANDLE hProcess)
{
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProcess, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}

	return addr;
}
