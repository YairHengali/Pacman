#ifndef _GHOST_H_
#define _GHOST_H_

#include "Creature.h"
#include <queue>
#include <vector>

class Ghost : public Creature
{
	bool isValidStep(const std::vector<std::vector<bool>> visit, int row, int col, Board& board) const;
	Position BFS(Board& board, std::vector<std::vector<bool>> visit, int row, int col) const;
protected:
	void smartMove(Board& board, Position&& target, bool isSilent);
public:
	Ghost(int row, int col) : Creature('$', (rand() % 4) + 1)
	{
		initPos.setPosition({ row, col });
		position.setPosition({ row, col });
	}
	virtual void move(Board& board, Position&& target, bool isSilent) = 0;
	virtual ~Ghost() {}
};
#endif
