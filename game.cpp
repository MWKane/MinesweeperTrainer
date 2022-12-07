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
#define BLANK_FLAG 0x0E
#define BLANK_QUESTION 0x0D
#define MINE 0x8F
#define MINE_REVEALED 0x8A
#define MINE_EXPLODED 0xCC
#define MINE_FLAG 0x8E
#define MINE_QUESTION 0x8D

// Memory addresses
#define BASE_ADDR 0x01000000
#define WIDTH BASE_ADDR + 0x5334
#define HEIGHT BASE_ADDR + 0x5338
#define MINEFIELD BASE_ADDR + 0x5340
#define CLOCK BASE_ADDR + 0x579C
#define INC_CLOCK BASE_ADDR + 0x2FF5
#define FUNC_REVEALMINES BASE_ADDR + 0x2F80
#define FUNC_WINGAME BASE_ADDR + 0x347C

// Hijacked functions
typedef void(__stdcall* _revealMines)(uint8_t lsb);
_revealMines revealMines;
typedef void(__stdcall* _winGame)(uint32_t arg_1);
_winGame winGame;

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

			case BLANK:
				val = ' ';
				color = GREY;
				break;

			case BLANK_FLAG:
				val = '!';
				color = ORANGE;
				break;

			case BLANK_QUESTION:
				val = '?';
				break;

			case MINE:
				val = '*';
				color = RED;
				break;

			case MINE_REVEALED:
				val = '*';
				color = ORANGE;
				break;

			case MINE_EXPLODED:
				val = '#';
				color = RED;
				break;

			case MINE_FLAG:
				val = '!';
				color = GREEN;
				break;

			case MINE_QUESTION:
				val = '?';
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

void game::WinGame()
{
	winGame = (_winGame)(FUNC_WINGAME);
	winGame(0x1);
}

CHAR CellToNumber(BYTE cell)
{
	int i = (int)cell - 0x10;
	return char(i);
}
