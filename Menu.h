#ifndef _MENU_H_
#define _MENU_H_

#include "io_utils.h"

class Menu
{
public:
	enum ghostIntelligence { BEST = '1', GOOD = '2', NOVICE = '3', GETBACK = '9' };
	enum menu { START = '1', STARTWITHCOLOR = '2', INSTRUCTIONS = '8', QUIT = '9' };
	enum chooseGameOptions { REGULAR = '1', SPECIFIC = '2', GET_BACK = '9' };

	void printMenu() const;
	void printInstructions() const;
	char manageMenu(char& chose, char& ghostIntel) const;
	void printGhostMoveOptions() const;
	char manageGhostMoveOptions() const;
	void printRegularOrSpecificOptions() const;
	char regularOrSpecific(char chose) const;
};

#endif