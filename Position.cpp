#include "Position.h"


//Prints given char on screen at position coordinates
void Position::draw(char ch) const
{
	gotoxy(x, y);
	std::cout << ch << std::endl;
}

//Changes position coordinates according to the given direction
void Position::move(int direction)
{
	switch (direction) {

	case Directions::UP: 
		--y;
		break;
	case Directions::DOWN:
		++y;
		break;
	case Directions::LEFT:
		--x;
		break;
	case Directions::RIGHT:
		++x;
		break;
	}
}

bool Position::operator ==(const Position& pos) const
{
	if (x == pos.getX() && y == pos.getY())
		return true;
	return false;
}