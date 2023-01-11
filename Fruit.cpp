#include "Fruit.h"

void Fruit::setInBoard(const Board& board)
{
	bool isfruitSet = false;

	while (!isfruitSet)
	{
		int randomX = rand() % board.getCols();
		int randomY = rand() % board.getRows();
		if (board.getCharInPosition(randomY, randomX) != WALL)
		{
			setPosition({ randomX, randomY });
			isfruitSet = true;
		}
	}
}