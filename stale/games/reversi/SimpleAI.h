#ifndef _SIMPLE_AI_H
#define _SIMPLE_AI_H

#include <engine/Player.h>

#include "ReversiGameState.h"

class SimpleAI : public Player
{
public:
	SimpleAI(ReversiGameState *state);
	virtual void receiveMsg(QString msg);
	virtual bool init();

private:
	QString makeMove();
	ReversiGameState *state;
};

#endif
