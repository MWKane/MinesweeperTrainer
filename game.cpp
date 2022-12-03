#include "pch.h"
#include "game.h"

// Colour codes
#define DEFAULT_COLOR 0x0F
#define MINE_COLOR 0x04
#define BLANK_COLOR 0x08
#define FLAG_BLANK_COLOR 0x06
#define FLAG_MINE_COLOR 0x0A
#define MINE_REVEALED_COLOR 0x06

// Cell values
#define BOUNDRY 0x10
#define EMPTY 0x0F
#define BLANK 0X40
#define MINE 0x8F
#define MINE_EXPLODED 0xCC
#define MINE_REVEALED 0x8A
#define FLAG_MINE 0x8E
#define FLAG_BLANK 0x0E
#define QUESTION 0x0D

// Memory addresses
#define BASE_ADDR 0x01000000
#define WIDTH BASE_ADDR + 0x5334
#define HEIGHT BASE_ADDR + 0x5338
#define MINEFIELD BASE_ADDR + 0x5340
#define CLOCK BASE_ADDR + 0x579C
#define INC_CLOCK BASE_ADDR + 0x2FF5

/// <summary>
/// Gets the CHAR number representation of the value of cell.
/// ie: Gets number of surrounding mines as a CHAR.
/// </summary>
/// <param name="cell"></param>
/// <returns></returns>
CHAR CellToNumber(BYTE cell);

void game::SaySomething(const char* msg)
{
	std::cout << msg << std::endl;
}

std::vector<std::vector<BYTE>> game::GetGrid()
{
	BYTE* minefield = (BYTE*)MINEFIELD;
	BYTE* width = (BYTE*)WIDTH;
	BYTE* height = (BYTE*)HEIGHT;

	uintptr_t rows = (uintptr_t)(*height);
	uintptr_t cols = (uintptr_t)(*width);

	std::vector<std::vector<BYTE>> grid(rows + 1, std::vector<BYTE>(cols + 1));
	for (unsigned int row = 1; row <= rows; row++)
	{
		for (unsigned int col = 1; col <= cols; col++)
		{
			BYTE* cell = minefield + col + (row * 0x20);
			grid[row][col] = *cell;
		}
	}

	return grid;
}

void game::DisplayGrid(std::vector<std::vector<BYTE>>& grid)
{
	size_t rows = grid.size();
	size_t cols = grid[0].size();

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (size_t row = 1; row < rows; row++)
	{
		for (size_t col = 1; col < cols; col++)
		{
			BYTE cell = grid[row][col];
			CHAR val = '\0';
			int color = DEFAULT_COLOR;

			switch (cell)
			{
			case EMPTY:
				val = ' ';
				break;

			case MINE:
				val = '*';
				color = MINE_COLOR;
				break;

			case BLANK:
				val = ' ';
				color = BLANK_COLOR;
				break;

			case FLAG_BLANK:
				val = '!';
				color = FLAG_BLANK_COLOR;
				break;

			case FLAG_MINE:
				val = '!';
				color = FLAG_MINE_COLOR;
				break;

			case QUESTION:
				val = '\?';
				break;

			case MINE_EXPLODED:
				val = '#';
				break;

			case MINE_REVEALED:
				val = '*';
				color = MINE_REVEALED_COLOR;
				break;

			default:
				val = CellToNumber(cell);
				break;
			}

			SetConsoleTextAttribute(hConsole, color);
			std::cout << '[' << val << ']';
		}

		std::cout << std::endl;
	}

	SetConsoleTextAttribute(hConsole, DEFAULT_COLOR);
}

void game::FreezeClock()
{
	mem::Nop((BYTE*)(INC_CLOCK), 6);
}

void game::UnfreezeClock()
{
	mem::Patch((BYTE*)INC_CLOCK, (BYTE*)"\xFF\x05\x9C\x57\x00\x01", 6);
}

BYTE game::SetClock(BYTE seconds)
{
	BYTE* clock = (BYTE*)CLOCK;
	BYTE old = *clock;

	*clock = seconds;
	return old;
}

CHAR CellToNumber(BYTE cell)
{
	int i = (int)cell - 0x10;
	return char(i);
}
