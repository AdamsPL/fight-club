#include "Player.h"

#include "GameState.h"

#include <QDebug>

Player::Player(GameState *state)
	: QObject(0), state(state), id(state->getPlayer())
{
}

void Player::sendMsg(QString msg)
{
	emit sendMsgSignal(id, msg);
}

void Player::receiveMsg(QString msg)
{
	emit msgReceivedSignal(msg);
}

bool Player::init()
{
	return true;
}

bool Player::cleanup()
{
	return true;
}

void Player::leave()
{
	emit playerLeftSignal(id);
}

QString Player::getName()
{
	return QString();
}
