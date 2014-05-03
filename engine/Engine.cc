#include "Engine.h"

#include "ExternalPlayer.h"
#include "PlayerListener.h"
#include "GameRules.h"
#include "GameState.h"

#include <QApplication>
#include <QTextStream>

Engine::Engine(GameRules *rules)
	: rules(rules)
{
}

bool Engine::registerPlayer(Player *player)
{
	connect(player, SIGNAL(sendMsgSignal(int, QString, int)), this, SLOT(receiveFromPlayer(int, QString, int)), Qt::QueuedConnection);
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

void Engine::receiveFromPlayer(int id, QString msg, int elapsed)
{
	PlayerListener *pl;
	GameResult gameResult;
	QTextStream out(stdout);
	MoveResult moveResult;

	msg = msg.toLower();

	foreach (pl, listeners) {
		pl->receiveMsg(id, msg);
	}
	
	if (!rules) {
		sendToPlayer(getNextPlayer(id), msg);
		return;
	}

	out << getPlayerName(id) << " : " << msg << " [" << elapsed << " ms]" << endl;
	
	moveResult = rules->validateMove(id, msg, elapsed);
	switch(moveResult) {
		case InvalidMove: out << "Warning!" << getPlayerName(id) << "made an invalid move! (" << msg << ")" << endl; break;
		case Timeout: out << "Warning!" << getPlayerName(id) << " exceeded his game time!" << endl; break;
		case ValidMove: break;
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
	QStringList params;
	GameState *state = rules->createGameState(id);

	params = getParamList(rules->getPlayerParamName(id));
	params << rules->getExternalPlayerArgs(id);
	ExternalPlayer *player = new ExternalPlayer(state, params);

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
	QTextStream out(stdout);

	out << "Warning!" << getPlayerName(id) << "has left the game" << endl;
	
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
	QTextStream out(stdout);

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

	out << "-------------------" << endl;
	out << msg << ' ' << p0 << " vs " << p1 << endl;
	out << "-------------------" << endl;

	stop(res);
}

QString Engine::getPlayerName(int player)
{
	QString label = QString("[%1]").arg(player);
	if (!players.contains(player))
		return label;

	return players[player]->getName() + label;
}

bool Engine::parseArgs(QStringList args)
{
	QString lastParam;
	QString tmp;

	foreach (tmp, args) {
		if (tmp.startsWith("--")) {
			lastParam = tmp.remove("--");
			parameters[lastParam] = QStringList();
			continue;
		}
		if (lastParam.isEmpty())
			continue;

		parameters[lastParam] << tmp;
	}
	if (!rules->readParams(this))
		return false;
	return true;
}

QString Engine::getParam(QString name)
{
	if (!parameters.contains(name))
		return QString();
	if (parameters[name].isEmpty())
		return QString();
	return parameters[name].first();
}

QStringList Engine::getParamList(QString name)
{
	if (!parameters.contains(name))
		return QStringList();
	return parameters[name];
}
