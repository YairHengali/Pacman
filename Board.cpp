#include "Board.h"

void Board::printBoard() const
{
	gotoxy(0, 0);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (board[(i * cols) + j] == BREADCRUMB)
				setTextColor(breadcrumbColor);
			else if (board[(i * cols) + j] == WALL)
				setTextColor(wallColor);

			std::cout << board[(i * cols) + j];
		}
		std::cout << std::endl;
	}
}