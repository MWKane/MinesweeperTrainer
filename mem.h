#pragma once
#include "pch.h"
#include <Windows.h>
#include <vector>

constexpr auto NOP = 0x90;

namespace mem
{
	/// <summary>
	/// Copies the values of size bytes from src to dst in the calling process.
	/// </summary>
	/// <param name="dst"></param>
	/// <param name="src"></param>
	/// <param name="size"></param>
	void Patch(BYTE* dst, BYTE* src, unsigned int size);

	/// <summary>
	/// Copies the values of size bytes from src to dst in the hProcess process.
	/// </summary>
	/// <param name="dst"></param>
	/// <param name="src"></param>
	/// <param name="size"></param>
	/// <param name="hProcess"></param>
	void PatchEx(BYTE* dst, BYTE* src, unsigned int size, HANDLE hProcess);

	/// <summary>
	/// Replaces instructions of size bytes at dst to NOP in the current process.
	/// </summary>
	/// <param name="dst"></param>
	/// <param name="size"></param>
	void Nop(BYTE* dst, unsigned int size);

	/// <summary>
	/// Replaces instructions of size bytes at dst to NOP in the hProcess process.
	/// </summary>
	/// <param name="dst"></param>
	/// <param name="size"></param>
	/// <param name="hProcess"></param>
	void NopEx(BYTE* dst, unsigned int size, HANDLE hProcess);

	/// <summary>
	/// Finds the address of a multilevel pointer in the current process.
	/// </summary>
	/// <param name="ptr"></param>
	/// <param name="offsets"></param>
	/// <returns></returns>
	uintptr_t FindMultilevelPointer(uintptr_t ptr, std::vector<unsigned int> offsets);

	/// <summary>
	/// Finds the address of a multilevel pointer in the hProcess process.
	/// </summary>
	/// <param name="ptr"></param>
	/// <param name="offsets"></param>
	/// <param name="hProcess"></param>
	/// <returns></returns>
	uintptr_t FindMultilevelPointerEx(uintptr_t ptr, std::vector<unsigned int> offsets, HANDLE hProcess);
}