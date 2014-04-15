#ifndef _GAME_RULES_H
#define _GAME_RULES_H

#include "Engine.h"

#include <QStringList>
#include <QMap>

class GameState;

class GameRules
{
public:
	bool parseArgs(QStringList args);

	virtual bool init() = 0;
	virtual QStringList getExternalPlayerArgs(int player) = 0;
	virtual GameState *createGameState(int player) = 0;
	virtual Engine::MoveResult validateMove(int player, QString move, int elapsed) = 0;
	virtual void onPlayerLeave(int player) = 0;
	virtual int getPoints(int player) = 0;
	virtual Engine::GameResult getGameResult() = 0;

protected:
	QString getParam(QString name);
	QStringList getParamList(QString name);
	QString getPlayerParamName(int player);

private:
	virtual bool checkParams() = 0;
	QMap<QString, QStringList> parameters;
};

#endif
