#include "GameRules.h"

GameRules::GameRules()
	: result(Engine::Undefined)
{
}

QString GameRules::getPlayerParamName(int player)
{
	static const QString prefix = "player";
	return prefix + QString::number(player);
}

void GameRules::setGameResult(Engine::GameResult res)
{
	result = res;
}

Engine::GameResult GameRules::getGameResult()
{
	return result;
}
