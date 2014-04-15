#include "SimpleAI.h"

#include <QApplication>

SimpleAI::SimpleAI(ReversiGameState *state)
	: Player(state), state(state)
{
}

void SimpleAI::receiveMsg(QString msg)
{
	state->makeMove(msg);
	sendMsg(makeMove());
}

QString SimpleAI::makeMove()
{
	int bestPoints = 0;
	int bestX = -1;
	int bestY = -1;
	int boardSize = state->getBoardSize();
	QString move;

	for (int x = 0; x < boardSize; ++x) { 
		for (int y = 0; y < boardSize; ++y) {
			int tmp = state->flip(x, y, true);
			if (tmp > bestPoints) {
				bestX = x;
				bestY = y;
				bestPoints = tmp;
			}
		}
	}
	if (bestX == -1 && bestY == -1)
		move = ReversiGameState::PASS;
	else
		move = ReversiGameState::convertToMove(bestX, bestY);

	state->makeMove(move);

	return move;
}

bool SimpleAI::init()
{
	if (getGameState()->getPlayer() == ReversiGameState::BLACK_PLAYER)
		sendMsg(makeMove());

	return true;
}
