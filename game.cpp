#include "pch.h"
#include "game.h"

// Values
#define BOUNDRY 0x10
#define EMPTY 0x0F
#define MINE 0x8F
#define BLANK 0X40
#define FLAG 0x8E
#define QUESTION 0x0D
#define EXPLOSION 0xCC

// Addresses
#define ROWS 0x01005334
#define COLS 0x01005338
#define MINEFIELD 0x01005340

std::vector<std::vector<BYTE>> game::GetGrid()
{
	std::vector<std::vector<BYTE>> grid(ROWS, std::vector<BYTE>(COLS));

	BYTE* minefield = (BYTE*)MINEFIELD;
	BYTE* width = (BYTE*)ROWS;
	BYTE* height = (BYTE*)COLS;

	uintptr_t rows = (uintptr_t)width;
	uintptr_t cols = (uintptr_t)height;

	for (unsigned int row = 1; row < rows; row++)
	{
		for (unsigned int col = 1; col < cols; col++)
		{
			BYTE* cell = minefield + row + (col * 0x20);
			grid[row][col] = *cell;
		}
	}

	return grid;
}

void game::DisplayGrid(std::vector<std::vector<BYTE>>& grid)
{
	size_t rows = grid.size();
	size_t cols = grid[0].size();

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			BYTE cell = grid[i][j];
			CHAR val = '\0';

			switch (cell)
			{
			case EMPTY:
				val = ' ';
				break;

			case MINE:
				val = '*';
				break;

			case BLANK:
				val = ' ';
				break;

			case FLAG:
				val = '!';
				break;

			case QUESTION:
				val = '?';
				break;

			case EXPLOSION:
				val = '#';
				break;

			default:
				val = char(cell);
				break;
			}

			std::cout << '[';
			std::cout << val;
			std::cout << ']';
		}

		std::cout << std::endl;
	}
}
