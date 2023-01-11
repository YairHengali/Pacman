#ifndef _NOVICEGHOST_H_
#define _NOVICEGHOST_H_

#include "Ghost.h"

class NoviceGhost : public Ghost
{
public:
	NoviceGhost(int row, int col) : Ghost(row, col) {}
	void move(Board& board, Position&& target, bool isSilent) override;
};
#endif
