#include "Pacman.h"

void Pacman::move(Board& board, bool isSilent)
{
	if (!isSilent) 
		position.draw(' ');
	position.move(direction);
	moveInTunnel(board);
	if (!isSilent)
	{
		setTextColor(color);
		position.draw(figure);
	}
}

void Pacman::blockByTunnel(int x, int y)
{
	position.setPosition(x, y);
	setDirection(Directions::STAY);
}

//Checkes whether pacman is in tunnel and if it is valid set him on the other side of it
void Pacman::moveInTunnel(Board& board)
{
	int y = position.getY();
	int x = position.getX();
	int rows = board.getRows();
	int cols = board.getCols();
	if (direction == Directions::UP && y < 0)
	{
		if (board.getCharInPosition(rows - 1, x) != WALL)
			position.setPosition(x, rows - 1);
		else
		{
			blockByTunnel(x, 0);
		}
	}
	else if (direction == Directions::LEFT && x < 0)
	{ 
		if (board.getCharInPosition(y, cols - 1) != WALL)
			position.setPosition(cols - 1, y);
		else
		{
			blockByTunnel(0, y);
		}
	}
	else if (direction == Directions::DOWN && y > rows -1)
	{
		if (board.getCharInPosition(0, x) != WALL)
			position.setPosition(x, 0);
		else
		{
			blockByTunnel(x, rows - 1);
		}
	}
		
	else if (direction == Directions::RIGHT && x > cols - 1)
	{
		if (board.getCharInPosition(y, 0) != WALL)
			position.setPosition(0, y);
		else
		{
			blockByTunnel(cols - 1, y);
		}
	}
}

//Gets a key and return the suitable direction
int Pacman::keyToDirection(char key) const
{
	if (key == 's' || key == 'S')
		return Directions::STAY;
	else if (key == 'w' || key == 'W')
		return Directions::UP;
	else if (key == 'a' || key == 'A')
		return Directions::LEFT;
	else if (key == 'x' || key == 'X')
		return Directions::DOWN;
	else if (key == 'd' || key == 'D')
		return Directions::RIGHT;
	else
		return -1;
}

//Checks if pacman can move according the wanted direction
bool Pacman::willMove(const Board& board, int dir) const
{
	int y = position.getY();
	int x = position.getX();
	int rows = board.getRows();
	int cols = board.getCols();

	if ((dir == Directions::RIGHT && x < cols - 1 && board.getCharInPosition(y, x + 1) == WALL) ||
		(dir == Directions::LEFT && x > 0 && board.getCharInPosition(y, x - 1) == WALL) ||
		(dir == Directions::UP && y > 0 && board.getCharInPosition(y - 1, x) == WALL) ||
		(dir == Directions::DOWN && y < rows - 1 && board.getCharInPosition(y + 1, x) == WALL))
		return false;

	return true;
}