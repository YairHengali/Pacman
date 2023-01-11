#include "SmartGhost.h"

void SmartGhost::move(Board& board, Position&& target, bool isSilent)
{
	smartMove(board, std::move(target), isSilent);
}