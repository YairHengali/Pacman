#include "GoodGhost.h"

void GoodGhost::move(Board& board, Position&& target, bool isSilent)
{
	if (moves < SWITCH_TO_NOVICE)
		smartMove(board, std::move(target), isSilent); //For 20 moves
	else
	{
		while (!(canMove(board)) || hasBordersInWay(board)) {
			setDirection(rand() % 4 + 1);
		}

		if (!isSilent)
			movementDraw(board);
		else
			position.move(direction);

		if (!isSilent) Sleep(20); //To balance game speed due to ghosts intelegence algoritm changes
		if (moves == SWITCH_TO_SMART)
			moves = 0;
	}


	moves++;
}