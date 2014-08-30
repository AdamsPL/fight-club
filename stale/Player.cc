#include "Player.h"

#include "GameState.h"

Player::Player(GameState *state)
	: QObject(0), state(state), id(state->getPlayer())
{
}

void Player::sendMsg(QString msg, int elapsed)
{
	emit sendMsgSignal(id, msg, elapsed);
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
