#include <wtypes.h>
#include "mem.h"
#pragma once

#define GAME_NAME "GAME.EXE"

// Include game specific functions here
namespace game
{
	void SaySomething(const char* msg);

	/// <summary>
	/// Gets a vector matrix of bytes representing the minefield.
	/// </summary>
	/// <param name="addr"></param>
	/// <returns></returns>
	std::vector<std::vector<BYTE>> GetGrid();

	/// <summary>
	/// Outputs the passed grid to the console.
	/// </summary>
	/// <param name="grid"></param>
	/// <param name="rows"></param>
	/// <param name="cols"></param>
	void DisplayGrid(std::vector<std::vector<BYTE>> &grid);

	/// <summary>
	/// Prevent the clock from increasing.
	/// </summary>
	void FreezeClock();

	/// <summary>
	/// Allow the clock to increase.
	/// </summary>
	void UnfreezeClock();

	/// <summary>
	/// Set the clock's value.
	/// </summary>
	/// <param name="seconds"></param>
	/// <returns>The clock's old value.</returns>
	BYTE SetClock(BYTE seconds);
}