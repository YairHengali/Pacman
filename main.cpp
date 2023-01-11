#include <iostream>

#include "ThePacmanGame.h"

int main(int argc, char* argv[])
{
		char play;
		ThePacmanGame game;
		game.insertToBoardsFiles(); //Get screen files in the working directory
		if (game.getBoardsFilesAmount())
		{
			game.checkInputParameters(argc, argv);
			do {
					play = game.startGame();
			} while (play != Menu::menu::QUIT);
		}
		else //If didnt found any screen file
		{
			std::cout << "Missing screen files" << std::endl;
			system("pause");
		}
}