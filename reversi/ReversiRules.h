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
	virtual bool init();
	virtual int getPoints(int player);
	virtual Engine::GameResult getGameResult();

	bool parsePlayerArgs(QStringList args);

	inline int getBoardSize() const { return 2 * boardSize; }
	inline int getPlayer() const { return player; }

private:
	virtual bool checkParams();
	inline int getNextPlayer(int player) { return (player + 1) % 2; };
    void broadcastState();	
	Engine::MoveResult returnMoveResult(Engine::MoveResult res, Engine::GameResult gameRes);

	int boardSize;
	int player;
	bool hasUI;

	QString lastMove;
	Engine::GameResult gameResult;
	ReversiGameState *state;
	ReversiGameWindow *ui;
    WebsocketServer *server;

	bool checkTime;
	int timeLeft[2];
};

#endif

