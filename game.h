#include <wtypes.h>
#pragma once

#define GAME_NAME "GAME.EXE"

// Include game specific functions here
namespace game
{
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
}