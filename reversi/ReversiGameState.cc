#include "ReversiGameState.h"

#include <iostream>
#include <QStringList>

#include <QDebug>

const QString ReversiGameState::PASS = "pass";

ReversiGameState::ReversiGameState(int player, int boardSize)
	: GameState(player), boardSize(boardSize), board(NULL)
{
	board = new int[boardSize * boardSize];
	for (int i = 0; i < boardSize * boardSize; ++i)
		board[i] = EMPTY;

	int half = boardSize / 2;

	setStatus(half - 1, half - 1, WHITE_PLAYER);
	setStatus(half, half - 1, BLACK_PLAYER);
	setStatus(half - 1, half, BLACK_PLAYER);
	setStatus(half, half, WHITE_PLAYER);
}

ReversiGameState::~ReversiGameState()
{
	delete[] board;
}

int ReversiGameState::getPoints(int player)
{
	int count = 0;

	for (int i = 0; i < boardSize * boardSize; ++i)
		count += (board[i] == player);

	return count;
}

int ReversiGameState::getEmpty()
{
	int count = 0;

	for (int i = 0; i < boardSize * boardSize; ++i)
		count += (board[i] == EMPTY);

	return count;
}

int ReversiGameState::getStatus(int x, int y) const
{
	if (x < 0 || y < 0 || x >= boardSize || y >= boardSize)
		return INVALID;
	int index = getIndex(x, y);
	return board[getIndex(x, y)];
}

void ReversiGameState::setStatus(int x, int y, int status)
{
	board[getIndex(x, y)] = status;
}

int ReversiGameState::flip(int x, int y, bool dryRun)
{
	int count = 0;

	if (getStatus(x, y) != EMPTY)
		return count;

	for (char dx = -1; dx <= 1; ++dx) {
		for (char dy = -1; dy <= 1; ++dy) {
			count += flipInDir(x + dx, y + dy, dx, dy, 0, dryRun);
		}
	}

	return count;
}

int ReversiGameState::flipInDir(int x, int y, char dx, char dy, int length, bool dryRun)
{
	int curPlayer = currentPlayer;
	int nextPlayer = getNextPlayer();
	int status = getStatus(x, y);

	if (status == curPlayer) {
		return length;
	}
	if (status == nextPlayer) {
		int result = flipInDir(x + dx, y + dy, dx, dy, length + 1, dryRun);
		if (result > 0 && !dryRun) {
			setStatus(x, y, curPlayer);
		}
		return result;
	}
	return 0;
}

bool ReversiGameState::hasPossibleMoves()
{
	for (int i = 0; i < boardSize; ++i)
		for (int j = 0; j < boardSize; ++j)
			if (flip(i, j, true) > 0)
				return true;
	return false;
}

int ReversiGameState::getNextPlayer()
{
	return (currentPlayer + 1) % 2;
}

bool ReversiGameState::makeMove(QString move)
{
	if (move == PASS) {
		if (hasPossibleMoves())
			return false;
		switchPlayer();
		return true;
	}

	QStringList list = move.split(" ");
	if (list.size() < 2)
		return false;

	int x = list[0].toInt();
	int y = list[1].toInt();

	if (flip(x, y) == 0)
		return false;

	setStatus(x, y, currentPlayer);
	switchPlayer();
	return true;
}

void ReversiGameState::print()
{
	for (int i = -1; i <= boardSize; ++i) {
		for (int j = -1; j <= boardSize; ++j) {
			char c;
			switch (getStatus(j, i)) {
				case BLACK_PLAYER: c = 'b'; break;
				case WHITE_PLAYER: c = 'w'; break;
				case INVALID: c = ' '; break;
				default: c = '.'; break;

			}
			std::cout << c;
		}
		std::cout << std::endl;
	}

}

QString ReversiGameState::convertToMove(int x, int y)
{
	return QString("%1 %2").arg(x).arg(y);
}
