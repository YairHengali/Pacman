#include "Creature.h"

class Fruit : public Creature
{
	int value;

	public:
	void setInBoard(const Board& board);

	void initData()
	{
		value = (rand() % 5) + 5;
		setFigure(value + '0');
		direction = rand() % 4 + 1;
	}

	int getValue() const
	{
		return value;
	}
};