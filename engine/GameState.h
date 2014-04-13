#ifndef _GAME_STATE_H
#define _GAME_STATE_H

#include <QString>

class GameState
{
public:
	GameState(int player);

	virtual bool makeMove(QString move) = 0;
	virtual int getPoints(int player) = 0;
	virtual int getNextPlayer() = 0;

	inline void switchPlayer() { currentPlayer = getNextPlayer(); }
	inline int getPlayer() const { return player; }
	inline int getCurrentPlayer() const { return currentPlayer; }

protected:
	const int player;
	int currentPlayer;
};

#endif
