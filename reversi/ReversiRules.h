#ifndef _REVERSI_RULES_H
#define _REVERSI_RULES_H

#include <engine/GameRules.h>
#include <engine/WebsocketServer.h>

#include <QString>
#include <QStringList>

class ReversiGameState;
class ReversiGameWindow;
class WebsocketServer;

class ReversiRules : public GameRules
{
public:
	ReversiRules();

	virtual QStringList getExternalPlayerArgs(int player);
	virtual GameState *createGameState(int player);
	virtual Engine::MoveResult validateMove(int player, QString move);
	virtual bool init();
	virtual int getWinner();
	virtual int getPoints(int player);

	bool parsePlayerArgs(QStringList args);

	inline int getBoardSize() const { return 2 * boardSize; }
	inline int getPlayer() const { return player; }

private:
	virtual bool checkParams();
	inline int getNextPlayer(int player) { return (player + 1) % 2; };
    void broadcastState();	

	int boardSize;
	int player;
	QString lastMove;
	int winner;
	ReversiGameState *state;
	ReversiGameWindow *ui;
    WebsocketServer *server;
};

#endif

