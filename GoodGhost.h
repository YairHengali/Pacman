#ifndef _GOODGHOST_H_
#define _GOODGHOST_H_

#include "Ghost.h"

class GoodGhost : public Ghost
{
	static constexpr int SWITCH_TO_SMART = 24, SWITCH_TO_NOVICE = 20;
public:
	GoodGhost(int row, int col) : Ghost(row, col) {}
	void move(Board& board, Position&& target, bool isSilent) override;
};
#endif
