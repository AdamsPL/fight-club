#ifndef _GAME_RULES_H
#define _GAME_RULES_H

#include "Engine.h"

#include <QStringList>
#include <QMap>

class GameState;

class GameRules
{
public:
	GameRules();

	virtual QStringList getExternalPlayerArgs(int player) = 0;
	virtual GameState *createGameState(int player) = 0;
	virtual Engine::MoveResult validateMove(int player, QString move, int elapsed) = 0;
	virtual void onPlayerLeave(int player) = 0;
	virtual int getPoints(int player) = 0;
	virtual bool readParams(Engine *engine) = 0;
	virtual int getNextPlayer(int player) = 0;

	Engine::GameResult getGameResult();
	QString getPlayerParamName(int player);

protected:
	void setGameResult(Engine::GameResult res);

private:
	Engine::GameResult result;
};

#endif
