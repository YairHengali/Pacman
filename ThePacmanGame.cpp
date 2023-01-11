#include "ThePacmanGame.h"

void ThePacmanGame::startSpecificBoard(char chose)
{
	std::string BoardName;
	bool foundFile;
	curLevel = boardsFiles.size();
	clear_screen();

	do
	{
		std::cout << "write 'back' To get back to main menu" << std::endl;
		std::cout << "Please enter the board file name:" << std::endl;
		std::cin >> BoardName;
		if (BoardName == "back") // If the user types back he will get back to main menu
		{
			clear_screen();
			return;
		}
		foundFile = std::find(boardsFiles.begin(), boardsFiles.end(), BoardName) != boardsFiles.end(); // searching for the explicit file
		if (foundFile)
			initBoard(BoardName);
		else if (!(BoardName.ends_with(".screen"))) // If the type of the file doesn't fit
		{
			std::cout << "It is not a valid screen file name\n" << std::endl;
		}
		else // If the requested file doesn't exist
		{
			std::cout << "File did not found\n" << std::endl;
		}
	} while (!foundFile);

	if (chose == Menu::menu::STARTWITHCOLOR)
	{
		setIsColor();
		setColors();
	}
	else
		setColors(Colors::LIGHTGREY);

	if (!isError)
	{
		clear_screen();
		initPositions();
		run();
	}
	else
		clear_screen();
}

void ThePacmanGame::checkInputParameters(int argc, char* argv[])
{
	if (argc == 1)
	{
		isLoad = isSave = isSilent = false;
		return;
	}

	if (argc == 2)
	{
		if (strcmp(argv[1], "-load") == 0)
			isLoad = true, isSave = isSilent = false;
		else if (strcmp(argv[1], "-save") == 0)
			isLoad = false, isSave = true, isSilent = false;
		else
			throw "Invalid arguments";
	}
	else if (argc == 3)
	{
		if (strcmp(argv[1], "-load") == 0 && strcmp(argv[2], "-silent") == 0)
			isLoad = true, isSave = false, isSilent = true;
		else if (strcmp(argv[1], "-save") == 0)
			isLoad = false, isSave = true, isSilent = false;
		else
			throw "Invalid arguments";
	}
}

char ThePacmanGame::startGame()
{
	if (isLoad)
	{
		try
		{
			init();
			if (isSilent)
				clear_screen();
			loadGame();
		}
		catch (const char* msg)
		{
			clear_screen();
			std::cout << msg << std::endl;
		}
		return Menu::menu::QUIT;
	}
	else {
		char chose, startGameOption;
		startGameOption = menu.manageMenu(chose, ghostIntel);

		if (chose != Menu::menu::QUIT)
		{
			if (startGameOption == Menu::chooseGameOptions::SPECIFIC) // Start specific screen
				startSpecificBoard(chose);
			else // Start all screen sequnce
			{
				curLevel = 1;
				if (chose == Menu::menu::STARTWITHCOLOR)
					setIsColor();

				clear_screen();

				init();
				run();
			}
		}
		return chose;
	}
}

void ThePacmanGame::printData(int row, int col) const
{
	if (isColorfull)
		setTextColor(Colors::WHITE);
	else
		setTextColor(Colors::LIGHTGREY);
	gotoxy(row, col);
	std::cout << "Remaining lives: " << pacman.getLives();
	gotoxy(row, col + 1);
	std::cout << "Current points:  " << points;
	setTextColor(Colors::LIGHTGREY);
}

void ThePacmanGame::printPauseMenu() const
{
	gotoxy(0, 0);
	std::cout << "--------------------------------- Game Paused ----------------------------------" << std::endl;
	std::cout << "(1) Press ESC to resume                                                         \n(2) Press G to get back to the menu                                             \n" << std::endl;
}

void ThePacmanGame::deletePauseMenu() //Delete first 3 lines and then print data
{
	gotoxy(0, 0);
	std::cout << "                                                                                " << std::endl;
	std::cout << "                                                                                " << std::endl;
	std::cout << "                                                                                " << std::endl;
	printData(dataPosition.getX(), dataPosition.getY());
	board.printBoard();
	if (isColorfull)
		setTextColor(Colors::YELLOW);
	if (!isSilent)
		pacman.getPosition().draw('@');
}

void ThePacmanGame::movePacman(int& dir, bool isSilent)
{
	if (dir != Directions::STAY)
	{
		if (!(pacman.canMove(board))) // If pacman cant move he will change his dirction
		{
			pacman.setDirection(Directions::STAY);
			if (isSave)
				insertDataToCreaturesSteps(pacman.getDirection(), 0);
		}
		else
		{
			pacman.move(board, isSilent);
			if (isSave)
				insertDataToCreaturesSteps(pacman.getDirection(), 0);
			pacmanHitBreadCrumb();
			if (pacmanHitGhost())
				dir = Directions::STAY;
			if (isThereFruit)
				pacmanHitFruit();
		}
	}
	else if (isSave)
		insertDataToCreaturesSteps(pacman.getDirection(), 0);
}

void ThePacmanGame::insertToStepsFile()
{
	int i;
	for (i = 0; i < creaturesSteps.size(); i++)
	{
		if (i == 0)
			stepsFile << "Pacman Steps:" << creaturesSteps[0] << "\n";
		else if (i < creaturesSteps.size() - 1)
			stepsFile << "Ghost " << i << " steps:" << creaturesSteps[i] << "\n";
		else
			stepsFile << "Fruit steps:" << creaturesSteps[i];
	}
}

void ThePacmanGame::insertDataToCreaturesSteps(int dir, int indexOfCreature)
{
	if (dir == Directions::STAY)
		creaturesSteps[indexOfCreature].append(" S");
	else if (dir == Directions::UP)
		creaturesSteps[indexOfCreature].append(" U");
	else if (dir == Directions::RIGHT)
		creaturesSteps[indexOfCreature].append(" R");
	else if (dir == Directions::DOWN)
		creaturesSteps[indexOfCreature].append(" D");
	else if (dir == Directions::LEFT)
		creaturesSteps[indexOfCreature].append(" L");
}

void ThePacmanGame::moveGhosts()
{
	size_t numOfGhosts = ghosts.size();
	int pacmanX = pacman.getX(), pacmanY = pacman.getY();
	for (int i = 0; i < numOfGhosts; i++) // For smart ghosts each ghost have a different mission
	{
		if (isLoad)
		{
			ghosts[i]->setDirection(getDataFromStepsFile(i + 1));
			if (!isSilent)
				ghosts[i]->movementDraw(board);
			else
				ghosts[i]->getPosition().move(ghosts[i]->getDirection());
		}
		else
		{
			if (i == 0)
				ghosts[i]->move(board, { pacmanX, pacmanY }, isSilent);
			else if (i == 1)
				ghosts[i]->move(board, { pacmanX + 2, pacmanY }, isSilent);
			else if (i == 2)
				ghosts[i]->move(board, { pacmanX, pacmanY + 2 }, isSilent);
			else if (i == 3)
				ghosts[i]->move(board, { pacmanX, pacmanY }, isSilent);

			if (isSave)
				insertDataToCreaturesSteps(ghosts[i]->getDirection(), i + 1);
		}
	}
	if (isThereFruit)
		ghostHitFruit();
}

void ThePacmanGame::fruitExistness() // This function able or disable the fruit appearence
{
	isThereFruit = !isThereFruit;
	if (isThereFruit)
		appearFruit();
	else
		disappearFruit();

	turns = 0;
	numOfFruitTurns = rand() % 200 + 50; // randon the number of turns of the fruit existness
}

void ThePacmanGame::updateFruitPos(int& xPos, int& yPos)
{
	char ch;
	stepsFile.get(ch);
	stepsFile.get(ch);
	for (int i = 0; i < 2; i++)
	{
		while (ch < '0' || ch > '9')
			stepsFile.get(ch);

		while (ch >= '0' && ch <= '9')
		{
			if (i == 0)
			{
				xPos *= 10;
				xPos += ch - '0';
				stepsFile.get(ch);
			}
			else
			{
				yPos *= 10;
				yPos += ch - '0';
				stepsFile.get(ch);
			}
		}
	}
}

// This function reset the fruit data
void ThePacmanGame::appearFruit()
{
	fruit.initData();
	if (!isLoad)
		fruit.setInBoard(board);
	else if (isLoad)
	{
		
		int xPos = 0, yPos = 0;
		updateFruitPos(xPos, yPos);
		fruit.setPosition({ xPos , yPos });
	}
	if (isSave)
	{
		creaturesSteps[creaturesSteps.size() - 1].append(" V ");
		creaturesSteps[creaturesSteps.size() - 1].append(std::to_string(fruit.getX()));
		creaturesSteps[creaturesSteps.size() - 1].append(" ");
		creaturesSteps[creaturesSteps.size() - 1].append(std::to_string(fruit.getY()));
	}
}

void ThePacmanGame::disappearFruit()
{
	int y = fruit.getPosition().getY();
	int x = fruit.getPosition().getX();

	if (!isSilent)
	{
		if (y < board.getRows() && x < board.getCols() && board.getCharInPosition(y, x) == BREADCRUMB)
		{
			setTextColor(board.getBreadcrumbColor());
			fruit.getPosition().draw(BREADCRUMB);
		}
		else
			fruit.getPosition().draw(SPACE);
	}
	fruit.setFigure(' ');
	fruit.setDirection(Directions::STAY);
	isThereFruit = false;
	if (isSave && time != 0)
		creaturesSteps[creaturesSteps.size() - 1].append(" X");
}

void ThePacmanGame::pacmanHitFruit()
{
	int x = pacman.getX(), y = pacman.getY();
	if (fruit.getPosition() == pacman.getPosition())
	{
		points += fruit.getValue(); //Add the fruit value to pacman gained points
		totalPoints += fruit.getValue();
		disappearFruit();
	}
}

void ThePacmanGame::ghostHitFruit()
{
	for (int i = 0; i < ghosts.size(); i++)
	{
		if (fruit.getPosition() == ghosts[i]->getPosition())
			disappearFruit();
	}
}

bool ThePacmanGame::pacmanHitGhost()
{
	int x = pacman.getX(), y = pacman.getY();
	for (int i = 0; i < ghosts.size(); i++)
	{
		if (pacman.getPosition() == ghosts[i]->getPosition())
		{
			initPositions();
			pacman.setLives(pacman.getLives() - 1); // Pacman lose 1 life when he hits a ghost
			if (isSave)
				resultFile << " " << time;

			if (isSilent)
				validiateResult();

			return true;
		}

	}
	return false;
}

void ThePacmanGame::pacmanHitBreadCrumb()
{
	int y = pacman.getY();
	int x = pacman.getX();
	if (board.getCharInPosition(y, x) == BREADCRUMB)
	{
		points++;
		board.setBoard(pacman.getY(), pacman.getX(), SPACE);
	}
}

void ThePacmanGame::init()
{
	initBoard(boardsFiles[curLevel - 1]); // Init board for current level

	if (!isError)
	{
		if (isColorfull)
			setColors();
		else
			setColors(Colors::LIGHTGREY);
		initPositions();
	}
}

// This function sets enough space to the data (lives and points)
void ThePacmanGame::makeSpaceForData(char& curChar, std::ifstream& boardFile)
{
	for (int i = 0; i < 20; i++)
	{
		board.pushBackBoard(SPACE);
		if (curChar != '\n')
			boardFile.get(curChar);
	}
}

// This function adds the appropriate spaces to the board vector in order to make the board a rectangle
void ThePacmanGame::dataInLastRow(std::ifstream& boardFile)
{
	int cols = board.getCols();
	for (int i = 0; i < (3 * cols - 20); i++)
		board.pushBackBoard(SPACE);
	board.setRows(board.getRows() + 3);
}

// This function adds all boards from current directory to the board vector
void ThePacmanGame::insertToBoardsFiles()
{
	std::string path;
	for (const auto& entry : fs::directory_iterator(current_path()))
	{
		path = entry.path().filename().string();
		if (path.ends_with(".screen"))
		{
			boardsFiles.push_back(path);
		}
	}
	std::sort(boardsFiles.begin(), boardsFiles.end());
}

// This function sets the board data according to the first line in the board
void ThePacmanGame::firstLineInBoard(std::ifstream& boardFile, char& curChar, int& curCol, int& curRow, bool& dataPosSet, bool& pacmanPosSet)
{
	while (curChar != '\n')
	{
		if (isError) return;
		if (dataPosSet && curChar == '&') // If the second data sign found
		{
			std::cout << "Only one data sign alowed!\n" << std::endl;
			isError = true;
			return;
		}
		if (curChar == '&') // If the data sign fount
		{
			dataPosSet = true;
			dataPosition.setPosition(curCol, curRow);
			makeSpaceForData(curChar, boardFile);
			curCol += TWENTY_SPACES;
		}
		else // Deals appropriatly to every given char in the file
		{
			board.pushBackBoard(convertChar(curChar, curRow, curCol, pacmanPosSet));
			curCol++;
			boardFile.get(curChar);
		}

	}
	curRow++;
}

bool ThePacmanGame::isInDataPreservedLocation(int curRow, int curCol, bool dataPosSet) const
{
	if (dataPosSet && dataPosition.getX() == curCol && curRow - dataPosition.getY() < THREE_LINES_SPACE_FOR_DATA)
		return true;
	return false;
}

void ThePacmanGame::checkNotFound(bool pacmanPosSet, bool dataPosSet)
{
	if (!dataPosSet)
	{
		std::cout << "No data found\n" << std::endl;
		isError = true;
		return;
	}
	if (!pacmanPosSet)
	{
		std::cout << "No Pacman found\n" << std::endl;
		isError = true;
		return;
	}
}
// This function create the rest of the board by the data of the first line
void ThePacmanGame::createRestOfBoard(std::ifstream& boardFile, char& curChar, int& curRow, bool& dataPosSet, bool& pacmanPosSet)
{
	int boardCols = board.getCols();
	int curCol;
	while (boardFile.peek() != EOF) // As long as the file isn't done keep reading
	{
		if (isError) return;
		boardFile.get(curChar);
		for (curCol = 0; curCol < boardCols; curCol++) // for loop by the number of colums of the first line
		{
			if (curChar == '\n') // If the line is short complete it
			{
				while (curCol < boardCols)
				{
					if (isInDataPreservedLocation(curRow, curCol, dataPosSet)) // Checks if the cur location in the data section
					{
						for (int i = 0; i < 20; i++)
							board.pushBackBoard(SPACE);
						curCol += 19;
					}
					else
					{
						board.pushBackBoard(BREADCRUMB);
						totalPoints++;
					}
					curCol++;
				}
			}
			else
			{
				if (dataPosSet && curChar == '&') // If the second data sign found
				{
					std::cout << "Only one data sign alowed!\n" << std::endl;
					isError = true;
					return;
				}
				if (curChar == '&') // data sign found
				{
					if (curCol + 19 > boardCols) // 19 or 20
					{
						std::cout << "Not enought colums for data!\n" << std::endl;
						isError = true;
						return;
					}
					dataPosSet = true;
					dataPosition.setPosition(curCol, curRow);
					makeSpaceForData(curChar, boardFile);
					curCol += 19;
					if (boardFile.peek() == EOF)
					{
						dataInLastRow(boardFile);
						return;
					}
				}
				// If the cursur in the file is in the same colum and up to three lines down from data ignore the file chars
				else if (isInDataPreservedLocation(curRow, curCol, dataPosSet))
				{
					makeSpaceForData(curChar, boardFile);
					curCol += 19;
				}
				else
				{
					board.pushBackBoard(convertChar(curChar, curRow, curCol, pacmanPosSet));
					boardFile.get(curChar);
				}
			}
		}
		while (curChar != '\n' && boardFile.peek() != EOF) // Ignore all chars the located after the max colum in the file
			boardFile.get(curChar);

		board.setRows(board.getRows() + 1); // Update the board rows
		curRow++;
		if (curRow > MAX_ROWS) // Too many rows
		{
			std::cout << "There are too many rows in the board!\n" << std::endl;
			isError = true;
			return;
		}
	}
}

// This function reads from file and create the board
void ThePacmanGame::initBoard(std::string fName)
{
	board.initBoard();
	std::ifstream boardFile(fName);
	if (!boardFile)
	{
		std::cout << "File could not be found!" << std::endl;
		return;
	}
	char curChar;
	int curCol = 0, curRow = 0;
	bool dataPosSet = false, pacmanPosSet = false;

	boardFile.get(curChar);
	if (curChar != '\n' && boardFile.peek() != EOF) // Checks if the board is nt empty
	{
		firstLineInBoard(boardFile, curChar, curCol, curRow, dataPosSet, pacmanPosSet);
		board.setCols(curCol);
		board.setRows(board.getRows() + 1);
		if (curCol > MAX_COLUMS) // Too many colums
		{
			std::cout << "There are too many colums in the board!\n" << std::endl;
			isError = true;
		}
		if (isError)
		{
			boardFile.close();
			system("pause");
			return;
		}

		createRestOfBoard(boardFile, curChar, curRow, dataPosSet, pacmanPosSet);
		if (isError)
		{
			boardFile.close();
			system("pause");
			return;
		}
		checkNotFound(pacmanPosSet, dataPosSet);
		if (isError)
		{
			boardFile.close();
			system("pause");
			return;
		}

		if (!isError && (isSave || isLoad))
		{
			if (isSave)
			{
				creaturesSteps.clear();
				creaturesSteps.resize(ghosts.size() + 2);
			}
			else
			{
				stepsFileInidces.clear();
				stepsFileInidces.resize(ghosts.size() + 2);
			}
			createRecordFiles(fName);
			time = 0;
		}
	}
	else
	{
		std::cout << "The board is empty!\n" << std::endl;
		system("pause");
	}
	boardFile.close();
}

void ThePacmanGame::createRecordFiles(std::string fName)
{
	std::string resultFileName = fName.replace(fName.size() - 6, fName.size(), "result");
	std::string stepsFileName = fName.replace(fName.size() - 6, fName.size(), "steps");

	if (isSave)
	{
		stepsFile.open(stepsFileName, std::fstream::out, std::ios::trunc);
		resultFile.open(resultFileName, std::fstream::out, std::ios::trunc);

		resultFile << "Points of time where pacman died:";
	}

	else if (isLoad)
	{
		stepsFile.open(stepsFileName, std::fstream::in);
		resultFile.open(resultFileName, std::fstream::in);

		char ch;
		std::string line;

		for (int i = 0; i < stepsFileInidces.size(); i++)
		{
			stepsFile.get(ch);
			while (ch != ':')
				stepsFile.get(ch);

			stepsFileInidces[i] = stepsFile.tellg();
			std::getline(stepsFile, line);
		}
	}

}

// This function gets a char from the file and convert to the appropriate data
char ThePacmanGame::convertChar(char ch, int curRow, int curCol, bool& pacmanPosSet)
{
	if (ch == '#')
		return WALL;
	else if (ch == SPACE)
	{
		totalPoints++;
		return BREADCRUMB;
	}
	else if (ch == '%')
		return SPACE;
	else if (ch == '$')
	{
		if (ghosts.size() < MAX_GHOSTS)
			initGhostVector(curCol, curRow);
		else if (ghosts.size() == MAX_GHOSTS) // If there are five ghosts
		{
			std::cout << "There are too many ghosts!\n" << std::endl;
			isError = true;
		}
		return SPACE;
	}
	else if (ch == '@')
	{
		if (!pacmanPosSet)
		{
			pacman.setPosition({ curCol, curRow });
			pacman.setInitPos({ curCol, curRow });
			pacmanPosSet = true;
		}
		else // If there are two Pacmans
		{
			std::cout << "Only one pacman alowed!\n" << std::endl;
			isError = true;
		}
		return SPACE;
	}
	else // If there is an invalid char
	{
		std::cout << "Invalid char in board!\n" << std::endl;
		isError = true;
		return SPACE;
	}
}

// This function insert the ghost vector the appropriate ghost by intelligence
void ThePacmanGame::initGhostVector(int curCol, int curRow) {
	if (ghostIntel == Menu::ghostIntelligence::BEST)
		ghosts.push_back(new SmartGhost(curCol, curRow));
	else if (ghostIntel == Menu::ghostIntelligence::GOOD)
		ghosts.push_back(new GoodGhost(curCol, curRow));
	else
		ghosts.push_back(new NoviceGhost(curCol, curRow));
}

void ThePacmanGame::initPositions() {
	disappearFruit();
	if (!isSilent)
		board.printBoard();
	if (isColorfull)
		setTextColor(Colors::YELLOW);
	for (int i = 0; i < ghosts.size(); i++)
		ghosts[i]->setPosition(ghosts[i]->getInitPos());

	pacman.setPosition(pacman.getInitPos());
	pacman.setDirection(Directions::STAY);
	if (!isSilent)
	{
		pacman.getPosition().draw('@');
		printData(dataPosition.getX(), dataPosition.getY());
	}
}

void ThePacmanGame::setColors() // Set colors to objects
{
	pacman.setColor(Colors::YELLOW);
	board.setBreadcrumbColor(Colors::CYAN);
	board.setWallColor(Colors::DARKGREY);
	for (int i = 0; i < ghosts.size(); i++)
		ghosts[i]->setColor(Colors::RED);
	fruit.setColor(Colors::BLUE);
}

void ThePacmanGame::setColors(Colors color) // reset color to objects
{
	pacman.setColor(color);
	board.setBreadcrumbColor(color);
	board.setWallColor(color);
	for (int i = 0; i < ghosts.size(); i++)
		ghosts[i]->setColor(color);
	fruit.setColor(color);
}

void ThePacmanGame::run()
{
	gameRun = true;
	paused = false;
	char key = 0;
	int dir = 0;
	while (pacman.getLives() > 0 && gameRun)
	{
		if (isError)
		{
			if (curLevel == boardsFiles.size()) //If the current level is the last one
			{
				gotoxy(0, 0);
				std::cout << "No more board. GAME OVER!                                                          " << std::endl;
				system("pause");
				endGame();
			}
			else
				moveToTheNextLevel(false);
		}
		else
		{
			if (_kbhit())
			{
				key = _getch();
				if (key == keys::ESC) { // If the user chose 'ESC' stop or resume
					paused = !paused;
					if (paused)
						printPauseMenu();
					else
						deletePauseMenu();
				}
				else if (paused && (key == keys::G || key == keys::g)) // If the user chose to quit game
					endGame();

				else if (!paused && (pacman.keyToDirection(key) != INVALID_MOVE)) { // If the game isn't paused and the key is valid the direction is set into dir
					dir = pacman.keyToDirection(key);
				}
			}
			if (!paused)
			{
				if (isSave)
					time++;

				if (pacman.willMove(board, dir)) // Checks if pacman can move according the wanted direction
					pacman.setDirection(dir); // Sets pacman direction by needed when the pacman can move that direction

				movePacman(dir, isSilent);

				if (letGhostsMove)
				{
					moveGhosts();
					if (pacmanHitGhost())
						dir = Directions::STAY;
				}
				letGhostsMove = !letGhostsMove;

				turns++;
				if (turns == numOfFruitTurns) // When it's time to show fruit
					fruitExistness();

				if (isThereFruit) // Check if the fruit is active
				{
					if (letFruitMove)
					{
						fruit.randomMove(board, isSilent);
						if (isSave)
							insertDataToCreaturesSteps(fruit.getDirection(), creaturesSteps.size() - 1);
						pacmanHitFruit();
						ghostHitFruit();
					}
					else if (isSave)
						creaturesSteps[creaturesSteps.size() - 1].append("  ");
					letFruitMove = !letFruitMove;
				}
				else if (isSave)
					creaturesSteps[creaturesSteps.size() - 1].append("  ");

				printData(dataPosition.getX(), dataPosition.getY());
				Sleep(20);
			}
		}

		if (gameRun && points == totalPoints)
		{
			if (isSave) //Insert to result file (passed the screen)
				resultFile << "\nPacman finished the screen at: " << time << ", reason: passed the screen";

			if (curLevel == boardsFiles.size()) //If the current level is the last one
			{
				gotoxy(0, 0);
				std::cout << "You did it! You won!!!                                                          " << std::endl;
				system("pause");
				endGame();
			}
			else
			{
				dir = Directions::STAY;
				moveToTheNextLevel(true);
			}

		}
	}

	if (pacman.getLives() == 0) // If the pacman is completly dead
	{
		if (isSave) //Insert to result file (game over)
			resultFile << "\nPacman finished the screen at: " << time << ", reason: Game Over";

		gotoxy(0, 0);
		std::cout << "Oh.. it seems like YOU LOST! GAME OVER!                                        " << std::endl;
		system("pause");

		endGame();
	}
}

void ThePacmanGame::deleteGhosts()
{
	for (int i = 0; i < ghosts.size(); i++)
		delete ghosts[i];
	ghosts.clear();
}

void ThePacmanGame::endGame()
{
	if (isSave || isLoad)
	{
		if (isSave)
			insertToStepsFile();
		if (isSilent)
		{
			validiateResult();
			std::cout << "TEST PASSED! \n";
			system("pause");
		}
		stepsFile.close();
		resultFile.close();
	}
	deleteGhosts();
	pacman.setLives(3);
	points = 0;
	totalPoints = 0;
	turns = 0;
	isColorfull = false;
	clear_screen();
	gameRun = false;
}

void ThePacmanGame::moveToTheNextLevel(bool isFinishedSuccesfully)
{
	if (isSave || isLoad)
	{
		if (isSave)
			insertToStepsFile();
		if (isSilent)
			validiateResult();
		stepsFile.close();
		resultFile.close();
	}
	deleteGhosts();
	gotoxy(0, 20);
	if (isFinishedSuccesfully && !isLoad)
	{
		std::cout << "You finished level " << curLevel << " succesfully!" << std::endl;
		system("pause");
	}
	else
		isError = false;
	curLevel++;
	turns = 0;
	clear_screen();
	init();
}

void ThePacmanGame::loadGame()
{
	gameRun = true;
	int dir = 0;
	while (pacman.getLives() > 0 && gameRun)
	{
		if (isError)
		{
			if (curLevel == boardsFiles.size()) //If the current level is the last one
			{
				gotoxy(0, 0);
				std::cout << "No more board. GAME OVER!                                                          " << std::endl;
				system("pause");
				endGame();
			}
			else
				moveToTheNextLevel(false);
		}
		else
		{
			time++;
			dir = getDataFromStepsFile(0);

			pacman.setDirection(dir); //Sets pacman direction by needed when the pacman can move that direction

			movePacman(dir, isSilent);

			if (letGhostsMove)
			{
				moveGhosts();
				pacmanHitGhost();
			}
			letGhostsMove = !letGhostsMove;

			fruit.setDirection(getDataFromStepsFile(stepsFileInidces.size() - 1));
			if (!isSilent && (fruit.getDirection() != Directions::STAY))
				fruit.movementDraw(board); 
			else if (isSilent)
				fruit.getPosition().move(fruit.getDirection());

			if (isThereFruit)
			{
				pacmanHitFruit();
				ghostHitFruit();
			}
			letFruitMove = !letFruitMove;

			if (!isSilent)
			{
				printData(dataPosition.getX(), dataPosition.getY());
				Sleep(20);
			}
			if (gameRun && points == totalPoints)
			{
				if (curLevel == boardsFiles.size()) //If the current level is the last one	
				{
					if (!isSilent)
					{
						gotoxy(0, 0);
						std::cout << "You did it! You won!!!                                                          " << std::endl;
						system("pause");
					}
					endGame();
				}
				else
				{
					dir = Directions::STAY;
					moveToTheNextLevel(true);
				}
			}
		}
	}

	if (pacman.getLives() == 0) // If the pacman is completly dead
	{
		if (!isSilent)
		{
			gotoxy(0, 0);
			std::cout << "Oh.. it seems like YOU LOST! GAME OVER!                                        " << std::endl;
			system("pause");
		}
		endGame();
	}
}

int ThePacmanGame::getDataFromStepsFile(int indexOfCreature)
{
	char ch;
	stepsFile.seekg(stepsFileInidces[indexOfCreature]);
	stepsFile.get(ch);
	stepsFile.get(ch);

	if (indexOfCreature == (stepsFileInidces.size() - 1)) //If the creature is Fruit
	{
		if (ch == 'V')
		{
			isThereFruit = true;
			appearFruit();
			stepsFile.get(ch);
		}
		else if (ch == 'X')
		{
			disappearFruit();
			stepsFile.get(ch);
			stepsFile.get(ch);
		}
	}

	stepsFileInidces[indexOfCreature] = stepsFile.tellg();

	if (ch == 'S' || ch == ' ' || ch == 'X')
		return Directions::STAY;
	else if (ch == 'U')
		return Directions::UP;
	else if (ch == 'R')
		return Directions::RIGHT;
	else if (ch == 'D')
		return Directions::DOWN;
	else
		return Directions::LEFT;

}

void ThePacmanGame::validiateResult()
{
	int fileTime = 0;
	char ch;
	resultFile.get(ch);
	while (ch < '0' || ch > '9')
		resultFile.get(ch);

	while (ch >= '0' && ch <= '9')
	{
		fileTime *= 10;
		fileTime += ch - '0';
		resultFile.get(ch);
	}

	if (fileTime != time)
	{
		deleteGhosts();
		throw "TEST FAILED";
	}
}
