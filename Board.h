#ifndef _BOARD_H_
#define _BOARD_H_

#include "io_utils.h"
#include "Position.h"

class Board
{
private:
	std::vector<char> board;
	int rows = 0, cols = 0;
	Colors breadcrumbColor = Colors::LIGHTGREY, wallColor = Colors::LIGHTGREY;

public:
	void initBoard()
	{
		board.clear();
		rows = 0;
		cols = 0;
	}

	void pushBackBoard(char ch)
	{
		board.push_back(ch);
	}

	int getRows() const
	{
		return rows;
	}
	void setRows(int nRows)
	{
		rows = nRows;
	}

	int getCols() const
	{
		return cols;
	}
	void setCols(int nCols)
	{
		cols = nCols;
	}

	char getCharInPosition(int i, int j) const
	{
		return board[(i * cols) + j];
	}
	void setBoard(int i, int j, char c)
	{
		board[(i * cols) + j] = c;
	};

	void setBreadcrumbColor(Colors newColor)
	{
		breadcrumbColor = newColor;
	};

	Colors getBreadcrumbColor() const
	{
		return breadcrumbColor;
	};

	void setWallColor(Colors newColor)
	{
		wallColor = newColor;
	};

	void printBoard() const;
};

#endif