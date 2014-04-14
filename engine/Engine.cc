#include "Engine.h"

#include "ExternalPlayer.h"
#include "PlayerListener.h"
#include "GameRules.h"
#include "GameState.h"

#include <QApplication>
#include <QDebug>

Engine::Engine(GameRules *rules)
	: rules(rules)
{
}

bool Engine::registerPlayer(Player *player)
{
	connect(player, SIGNAL(sendMsgSignal(int, QString)), this, SLOT(receiveFromPlayer(int, QString)), Qt::QueuedConnection);
	connect(player, SIGNAL(playerLeftSignal(int)), this, SLOT(onPlayerLeave(int)), Qt::QueuedConnection);
	players[player->getId()] = player;
	return player->init();
}

void Engine::unregisterPlayer(Player *player)
{
	if (!players.contains(player->getId()))
		return;
	players.remove(player->getId());
}

void Engine::sendToPlayer(int id, QString msg)
{
	if (!players.contains(id))
		return;
	players[id]->receiveMsg(msg);
}

void Engine::receiveFromPlayer(int id, QString msg)
{
	PlayerListener *pl;
	GameResult gameResult;

	msg = msg.toLower();

	foreach (pl, listeners) {
		pl->receiveMsg(id, msg);
	}

	
	if (!rules) {
		sendToPlayer(getNextPlayer(id), msg);
		return;
	}

	qDebug() << getPlayerName(id) << ':' << msg;
	
	if (!rules->validateMove(id, msg)) {
		qDebug() << "Warning!" << getPlayerName(id) << "made an invalid move! (" << msg << ")";
	}

	gameResult = rules->getGameResult();
	if (gameResult == Undefined) {
		sendToPlayer(getNextPlayer(id), msg);
		return;
	}

	printStats(gameResult);
}

void Engine::registerPlayerListener(PlayerListener *listener)
{
	if (listeners.contains(listener))
		return;
	listeners << listener;
}

void Engine::unregisterPlayerListener(PlayerListener *listener)
{
	if (!listeners.contains(listener))
		return;
	listeners.remove(listener);
}

Player *Engine::loadExternalPlayer(int id)
{
	GameState *state = rules->createGameState(id);
	ExternalPlayer *player = new ExternalPlayer(state, rules->getExternalPlayerArgs(id));
	return player;
}

int Engine::getNextPlayer(int player)
{
	if (players.isEmpty())
		return 0;

	return (player + 1) % players.size();
}

void Engine::onPlayerLeave(int id)
{
	qDebug() << "Warning!" << getPlayerName(id) << "has left the game";
	
	if (!rules) {
		unregisterPlayer(players[id]);
		stop(Tie);
	}

	rules->onPlayerLeave(id);
	unregisterPlayer(players[id]);
	printStats(rules->getGameResult());
}

void Engine::stop(GameResult res)
{
	Player *p;

	foreach (p, players.values()) {
		p->cleanup();
		unregisterPlayer(p);
	}
	QApplication::exit(res);
}

void Engine::printStats(GameResult res)
{
	if (!rules)
		return;

	int p0 = rules->getPoints(0);
	int p1 = rules->getPoints(1);
	QString msg;

	switch(res) {
		case Tie: msg = "Game ended with TIE"; break;
		case PlayerZeroWon: msg = getPlayerName(0) + " WON!"; break;
		case PlayerOneWon: msg = getPlayerName(1) + " WON!"; break;
		default: msg = QString("ERROR. Uknown game result: %1").arg(res); break;
	}

	qDebug() << "-------------------";
	qDebug() << msg << p0 << "vs" << p1;
	qDebug() << "-------------------";

	stop(res);
}

QString Engine::getPlayerName(int player)
{
	QString label = QString("[%1]").arg(player);
	if (!players.contains(player))
		return label;

	return players[player]->getName() + label;
}
