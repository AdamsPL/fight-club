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
	virtual Engine::MoveResult validateMove(int player, QString move, int elapsed);
	virtual void onPlayerLeave(int player);
	virtual int getPoints(int player);

	inline int getBoardSize() const { return 2 * boardSize; }
	inline int getPlayer() const { return player; }

	virtual bool readParams(Engine *engine);
	virtual int getNextPlayer(int player) { return (player + 1) % 2; };

	bool parsePlayerArgs(QStringList args);

private:
    //void broadcastState();	
	Engine::MoveResult returnMoveResult(Engine::MoveResult res, Engine::GameResult gameRes);

	int boardSize;
	int player;
	bool hasUI;

	QString lastMove;
	ReversiGameState *state;
	ReversiGameWindow *ui;
    //WebsocketServer *server;

	bool checkTime;
	int timeLeft[2];
};

#endif

