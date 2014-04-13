#ifndef _PLAYER_LISTENER_H
#define _PLAYER_LISTENER_H

#include <QString>

class PlayerListener
{
public:
	virtual void receiveMsg(int fromPlayer, QString msg) = 0;
};

#endif
