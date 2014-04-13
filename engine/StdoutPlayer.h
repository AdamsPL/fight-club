#ifndef _STDOUT_PLAYER_H
#define _STDOUT_PLAYER_H

#include "Player.h"

#include <QThread>

class Player;

class StdoutPlayer : public Player, public QThread
{
public:
	StdoutPlayer(GameState *state);
	~StdoutPlayer();

	virtual void receiveMsg(QString msg);

protected:
	virtual void run();

private:
	bool isRunning;
};

#endif
