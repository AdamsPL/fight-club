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
	virtual bool validateMove(int player, QString move);
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

	int boardSize;
	int player;
	bool hasUI;

	QString lastMove;
	Engine::GameResult gameResult;
	ReversiGameState *state;
	ReversiGameWindow *ui;
    WebsocketServer *server;
};

#endif

