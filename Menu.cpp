#include "Menu.h"

void Menu::printMenu() const
{
	std::cout << "Please select one of the options bellow:" << std::endl;
	std::cout << "(1) Start a new game\n(2) Start a new colorful game\n(8) Present instructions and keys\n(9) EXIT" << std::endl;
}

void Menu::printInstructions() const
{
	clear_screen();
	std::cout << "The game instructions Are:" << std::endl << "Playing Pacman is easy to learn and hard to master (like all classic games)." << std::endl;
	std::cout << "Simply score as many points as you can eating the small breadcrumbs all around the maze, and avoid the ghosts." << std::endl;
	std::cout << "The game keys Are:" << std::endl;
	std::cout << "w - Up\nd - Right\nx - Down\na - Left\ns - Stay\nEsc - Pause\n " << std::endl;
}

void Menu::printGhostMoveOptions() const
{
	clear_screen();
	std::cout << "How good are you at this game?" << std::endl;
	std::cout << "(1) - BEST\n(2) - GOOD\n(3) - NOVICE\n(9) - Get back" << std::endl;
}

void Menu::printRegularOrSpecificOptions() const
{
	clear_screen();
	std::cout << "Do you want to play a regular game or a specific board?" << std::endl;
	std::cout << "(1) - regular\n(2) - specific\n(9) - Get back" << std::endl;
}

char Menu::manageGhostMoveOptions() const
{
	printGhostMoveOptions();
	char chose = _getch();
	while (chose != ghostIntelligence::GETBACK) {
		if (chose == ghostIntelligence::BEST)
			return ghostIntelligence::BEST;
		else if (chose == ghostIntelligence::GOOD)
			return ghostIntelligence::GOOD;
		else if (chose == ghostIntelligence::NOVICE)
			return ghostIntelligence::NOVICE;
		else		//if user entered other input
		{
			std::cout << std::endl << "You entered a wrong input." << std::endl;
			chose = _getch();
		}
	}
	clear_screen();
	return ghostIntelligence::GETBACK;
}

char Menu::regularOrSpecific(char chose) const
{
	printRegularOrSpecificOptions();
	char input = _getch();
	while (input != chooseGameOptions::GET_BACK) {
		if (input == chooseGameOptions::REGULAR)
			return chooseGameOptions::REGULAR;
		else if (input == chooseGameOptions::SPECIFIC)
			return chooseGameOptions::SPECIFIC;
		else		//if user entered other input
		{
			std::cout << std::endl << "You entered a wrong input." << std::endl;
			input = _getch();
		}

	}
	clear_screen();
	return chooseGameOptions::GET_BACK;
}

char Menu::manageMenu(char& chose, char& ghostIntel) const
{
	printMenu();
	chose = _getch();
	char startGameOption;

	while (chose != menu::QUIT) {
		if (chose == menu::START || chose == menu::STARTWITHCOLOR)
		{
			while ((ghostIntel = manageGhostMoveOptions()) != ghostIntelligence::GETBACK)
			{
				startGameOption = regularOrSpecific(chose);
				if (startGameOption != chooseGameOptions::GET_BACK)
					return startGameOption;
			}
		}
		else if (chose == menu::INSTRUCTIONS)
		{
			printInstructions();
		}
		else if (chose != menu::QUIT)
		{
			std::cout << std::endl << "You entered a wrong input." << std::endl;
		}
		printMenu();
		chose = _getch();
	}
	return menu::QUIT;
}