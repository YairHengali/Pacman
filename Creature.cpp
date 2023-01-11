#include "Creature.h"

bool Creature::canMove(const Board& board) const	//Checks whether the creature can move or not
{
	int y = position.getY();
	int x = position.getX();
	int rows = board.getRows();
	int cols = board.getCols();

	//If has walls in his way
	if ((direction == Directions::RIGHT && x < cols - 1 && board.getCharInPosition(y, x + 1) == WALL) ||
		(direction == Directions::LEFT && x > 0 && board.getCharInPosition(y, x - 1) == WALL) ||
		(direction == Directions::UP && y > 0 && board.getCharInPosition(y - 1, x) == WALL) ||
		(direction == Directions::DOWN && y < rows - 1 && board.getCharInPosition(y + 1, x) == WALL))
		return false;

	return true;
}

bool Creature::hasBordersInWay(const Board& board) const //Checks whether the creature has borders in its  way
{
	int y = position.getY();
	int x = position.getX();

	//If has borders in his way
	if ((direction == Directions::RIGHT && x == +board.getCols() - 1) ||
		(direction == Directions::LEFT && x == 0) ||
		(direction == Directions::UP && y == 0) ||
		(direction == Directions::DOWN && y == board.getRows() - 1))
		return true;

	return false;
}

void Creature::randomMove(const Board& board, bool isSilent) 	//Execute random movement to a creature
{
	if (moves == TOTAL_MOVES)
	{
		setDirection(rand() % 4 + 1);
		moves = 0;
	}

	while (!(canMove(board)) || hasBordersInWay(board)) {
		setDirection(rand() % 4 + 1);
	}

	if (!isSilent)
		movementDraw(board);
	else
		position.move(direction);

	moves++;
}

void Creature::movementDraw(const Board& board) //Draw the creature in its new position according to its new direction
{
	if (board.getCharInPosition(position.getY(), position.getX()) == BREADCRUMB) //If there was breadcrumb in its old position
	{
		setTextColor(board.getBreadcrumbColor());
		position.draw(BREADCRUMB);
	}
	else
		position.draw(SPACE);
	position.move(direction);
	setTextColor(color);
	position.draw(figure);
}