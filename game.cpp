#include "pch.h"
#include "game.h"

// Colour codes
#define WHITE 0x0F
#define RED 0x04
#define GREY 0x08
#define ORANGE 0x06
#define GREEN 0x0A

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
#define FUNC_REVEALMINES BASE_ADDR + 0x2F80

// Hijacked functions
typedef void(__stdcall* _revealMines)(uint8_t lsb);
_revealMines revealMines;

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
			int color = WHITE;

			switch (cell)
			{
			case EMPTY:
				val = ' ';
				break;

			case MINE:
				val = '*';
				color = RED;
				break;

			case BLANK:
				val = ' ';
				color = GREY;
				break;

			case FLAG_BLANK:
				val = '!';
				color = ORANGE;
				break;

			case FLAG_MINE:
				val = '!';
				color = GREEN;
				break;

			case QUESTION:
				val = '\?';
				break;

			case MINE_EXPLODED:
				val = '#';
				break;

			case MINE_REVEALED:
				val = '*';
				color = ORANGE;
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

	SetConsoleTextAttribute(hConsole, WHITE);
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

void game::RevealMines()
{
	// LSB of 0xA = Revealed Mine
	// LSB of 0xE = Flagged Mine
	revealMines = (_revealMines)(FUNC_REVEALMINES);
	revealMines(0xE);
	//revealMines(0xA);
}

CHAR CellToNumber(BYTE cell)
{
	int i = (int)cell - 0x10;
	return char(i);
}
