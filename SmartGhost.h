#ifndef _SMARTGHOST_H_
#define _SMARTGHOST_H_

#include "Ghost.h"

class SmartGhost : public Ghost
{
public:
	SmartGhost(int row, int col) : Ghost(row, col) {}
	void move(Board& board, Position&& target, bool isSilent) override;
};
#endif
