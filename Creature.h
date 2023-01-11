#ifndef _CREATURE_H_
#define _CREATURE_H_


#include "Position.h"
#include "io_utils.h"
#include "Directions.h"
#include "Board.h"

class Creature {

protected:
	Position position, initPos;
	char figure = ' ';
	int direction = 0;
	Colors color = Colors::LIGHTGREY;
	int moves = 0;
	static constexpr int TOTAL_MOVES = 20;
	
public:
	Creature() {}
	Creature(char shape, int dir) : figure(shape), direction(dir) {}

	bool canMove(const Board& board) const;

	void randomMove(const Board& board, bool isSilent);

	void setFigure(char ch)
	{
		figure = ch;
	}

	int getDirection() const
	{
		return direction;
	}
	
	void setDirection(int dir)
	{
		direction = dir;
	}

	Position& getPosition()
	{
		return position;
	}

	void setPosition(const Position pos)
	{
		position.setPosition(pos);
	}

	Position& getInitPos()
	{
		return initPos;
	}

	void setInitPos(const Position pos)
	{
		initPos.setPosition(pos);
	}

	int getX() const
	{
		return position.getX();
	}
	int getY() const
	{
		return position.getY();
	}

	void setColor(Colors newColor)
	{
		color = newColor;
	}

	bool hasBordersInWay(const Board& board) const;

	void movementDraw(const Board& board);
};

#endif