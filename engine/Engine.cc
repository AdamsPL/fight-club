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
	MoveResult result = ValidMove;

	foreach (pl, listeners)
		pl->receiveMsg(id, msg);

	if (rules) {
		qDebug() << id << ':' << msg;
		result = rules->validateMove(id, msg);
	}
	switch(result) {
		case ValidMove: sendToPlayer(getNextPlayer(id), msg); break;
		case InvalidMove: qDebug() << "INVALID MOVE!"; stop();
		case FinishingMove: printStats(); stop(); break;
	}
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
	if (!players.contains(id))
		return;
	qDebug() << "Player" << id << " has left the game";
	unregisterPlayer(players[id]);

	if (players.count() == 0)
		stop();
}

void Engine::stop()
{
	Player *p;
	foreach (p, players.values()) {
		p->cleanup();
		unregisterPlayer(p);
	}
	QApplication::exit(0);
}

void Engine::printStats()
{
	if (!rules)
		return;
	int winner = rules->getWinner();
	int p0 = rules->getPoints(winner);
	int p1 = rules->getPoints(getNextPlayer(winner));
	qDebug() << "Player" << winner << "won!";
	qDebug() << p0 << "vs" << p1;
}
