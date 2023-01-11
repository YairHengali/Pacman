#ifndef _POSITION_H_
#define _POSITION_H_

#include "io_utils.h"
#include "Directions.h"

class Position
{
	int x, y;
public:
	Position(int _x = 1, int _y = 1) : x(_x), y(_y) {}

	void setPosition( int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	void setPosition(const Position& position)
	{
		x = position.x;
		y = position.y;
	}
	void draw(char ch) const;
	void move(int direction);
	int getX() const
	{
		return x;
	}
	int getY() const
	{
		return y;
	}

	bool operator ==(const Position& other) const;
};

#endif