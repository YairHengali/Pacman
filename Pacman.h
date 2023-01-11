#ifndef _PACMAN_H_
#define _PACMAN_H_

#include "Creature.h"

class Pacman : public Creature
{
	int lives = 3;

	void blockByTunnel(int x, int y);

public:
	Pacman() : Creature('@', Directions::STAY){}

	bool willMove(const Board& board, int dir) const;
	void move(Board& board, bool isSilent);
	void moveInTunnel(Board& board);
	
	int keyToDirection(char key) const;

	int getLives() const
	{
		return lives;
	}
	void setLives(int newLives)
	{
		lives = newLives;
	}
};

#endif