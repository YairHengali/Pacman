#include "NoviceGhost.h"

void NoviceGhost::move(Board& board, Position&& target, bool isSilent)
{
	randomMove(board, isSilent);
	if (!isSilent) Sleep(20); //To balance game speed due to ghosts intelegence algoritm changes
}