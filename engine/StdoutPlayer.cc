#include "StdoutPlayer.h"

#include <cstdio>

StdoutPlayer::StdoutPlayer(GameState *state)
	: Player(state), isRunning(true)
{
}

StdoutPlayer::~StdoutPlayer()
{
	isRunning = false;
	QThread::terminate();
	QThread::wait();
}

void StdoutPlayer::receiveMsg(QString msg)
{
	puts(msg.toAscii());
	fflush(stdout);
}

void StdoutPlayer::run()
{
	char buf[128];
	while(isRunning) {
		fgets(buf, sizeof(buf), stdin);
		sendMsg(QString::fromAscii(buf).simplified());
	}
}
