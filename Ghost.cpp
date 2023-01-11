#include "Ghost.h"

void Ghost::smartMove(Board& board, Position&& target, bool isSilent)
{
    int xGhostPos = position.getX(), yGhostPos = position.getY();
    int xTargetPos = target.getX(), yTargetPos = target.getY();
    std::vector<std::vector<bool>> visit(board.getRows() + 2, std::vector<bool>(board.getCols() + 2, false));
    Position newPos = BFS(board, visit, yTargetPos, xTargetPos);

    //newPos is to the left
    if (newPos.getX() == xGhostPos - 1) setDirection(Directions::LEFT);

    //newPos is to the right
    else if (newPos.getX() == xGhostPos + 1) setDirection(Directions::RIGHT);

    
    //newPos is to the up
    else if (newPos.getY() == yGhostPos - 1) setDirection(Directions::UP);

    //newPos is to the down
    else if (newPos.getY() == yGhostPos + 1) setDirection(Directions::DOWN);

    //if returned the same position, set random direction
    while (!(canMove(board)) || hasBordersInWay(board))
        setDirection(rand() % 4 + 1);

    //show movement on board according the chosen direction and mode
    if (!isSilent)
        movementDraw(board);
    else
        position.move(direction);
}

bool Ghost::isValidStep(const std::vector<std::vector<bool>> visit, int row, int col, Board& board) const
{
    // If cell is bounds
    if (row < 0 || row >= board.getRows() || col < 0 || col >= board.getCols())
        return false;

    // If cell is WALL
    if (board.getCharInPosition(row, col) == WALL)
        return false;

    // If cell is already visited
    if (visit[row][col])
        return false;

    // Otherwise
    return true;
}

Position Ghost::BFS(Board& board, std::vector<std::vector<bool>> visit, int row, int col) const
{
    //to chose adjacent cells
    int dRow[] = { -1, 0, 1, 0 };
    int dCol[] = { 0, 1, 0, -1 };

    // Stores indices of the matrix cells
    std::queue<Position> q;
    int xGhost = position.getX();
    int yGhost = position.getY();

    // Mark the starting cell as visited and push it into the queue
    q.push(Position{ col, row });
    visit[row][col] = true;

    // Iterate while the queue is not empty
    while (!q.empty()) {

        Position cell = q.front();
        q.pop();
        int x = cell.getX();
        int y = cell.getY();



        //check adjacent cells
        for (int i = 0; i < 4; i++) {
            int adjy = y + dRow[i];
            int adjx = x + dCol[i];
            if (Position(adjx, adjy) == position)
                return Position(x, y);
            else if (isValidStep(visit, adjy, adjx, board)) {
                q.push({ adjx, adjy });
                visit[adjy][adjx] = true;
            }
        }
    }

    //if didnt found a path, will return current position and will set a random direction
    return position;
}
