#ifndef _THEPACMANGAME_H_
#define _THEPACMANGAME_H_

#include <string>
#include "io_utils.h"
#include "Pacman.h"
#include "Ghost.h"
#include "SmartGhost.h"
#include "GoodGhost.h"
#include "NoviceGhost.h"
#include "Board.h"
#include "Fruit.h"
#include "Menu.h"
#include <fstream>
#include <filesystem>

using std::filesystem::current_path;
using std::filesystem::directory_iterator;
namespace fs = std::filesystem;

class ThePacmanGame {
	static constexpr int INVALID_MOVE = -1;
	static constexpr int MAX_GHOSTS = 4;
	static constexpr int MOVE_GHOST_IN_EVERY_TWENTY_MOVES = 20;
	static constexpr int MOVE_FRUIT_IN_EVERY_TEN_MOVES = 10;
	static constexpr int THREE_LINES_SPACE_FOR_DATA = 3;
	static constexpr int TWENTY_SPACES = 20;
	static constexpr int MAX_ROWS = 25;
	static constexpr int MAX_COLUMS = 80;
	int numOfFruitTurns = 50;
	char ghostIntel;
	enum keys { ESC = 27, G = 71, g = 103 };
	Menu menu;
	Pacman pacman;
	Position dataPosition;
	std::vector<Ghost*> ghosts;
	std::vector<std::string> boardsFiles;
	std::vector<std::string> creaturesSteps;
	std::fstream stepsFile;
	std::fstream resultFile;
	std::vector<int> stepsFileInidces;
	Fruit fruit;
	Board board;
	int points = 0, turns = 0, totalPoints = 0, curLevel = 1, time = 0;
	bool paused = false, letGhostsMove = true, isColorfull = false, gameRun = true, isThereFruit = false, letFruitMove = true, isLoad = false, isSave = false, isSilent = false, isError = false;

	void setIsColor()
	{
		isColorfull = true;
	};
	void init();
	void initBoard(std::string fName);
	void initPositions();
	void run();
	void printData(int row, int col) const;
	void printPauseMenu() const;
	void deletePauseMenu();
	void movePacman(int& dir, bool isSilent);
	void moveGhosts();
	bool pacmanHitGhost();
	void pacmanHitBreadCrumb();
	void setColors();
	void setColors(Colors color);
	char convertChar(char ch, int curRow, int curCol, bool& pacmanPosSet);
	void makeSpaceForData(char& curChar, std::ifstream& boardFile);
	void dataInLastRow(std::ifstream& boardFile);
	void firstLineInBoard(std::ifstream& boardFile, char& curChar, int& curCol, int& curRow, bool& dataPosSet, bool& pacmanPosSet);
	void createRestOfBoard(std::ifstream& boardFile, char& curChar, int& curRow, bool& dataPosSet, bool& pacmanPosSet);
	void checkNotFound(bool pacmanPosSet, bool dataPosSet);
	void fruitExistness();
	void updateFruitPos(int& xPos, int& yPos);
	void appearFruit();
	void disappearFruit();
	void pacmanHitFruit();
	void ghostHitFruit(); 
	void deleteGhosts();
	void endGame();
	void moveToTheNextLevel(bool isFinishedSuccesfully);
	void startSpecificBoard(char chose);
	void initGhostVector(int curCol, int curRow);
	bool isInDataPreservedLocation(int curRow, int curCol, bool dataPosSet) const;
	void createRecordFiles(std::string fName);
	void insertDataToCreaturesSteps(int dir, int indexOfCreature);
	void insertToStepsFile();
	void loadGame();
	int getDataFromStepsFile(int indexOfCreature);
	void validiateResult();

public:
	char startGame();
	void insertToBoardsFiles();
	void checkInputParameters(int argc, char* argv[]);
	size_t getBoardsFilesAmount() const
	{
		return boardsFiles.size();
	}
};
#endif
