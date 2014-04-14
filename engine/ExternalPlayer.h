#ifndef _EXTERNAL_PLAYER_H
#define _EXTERNAL_PLAYER_H

#include "Player.h"

#include <QProcess>

class ExternalPlayer : public Player
{
Q_OBJECT

public:
	ExternalPlayer(GameState *state, QStringList arguments);
	virtual ~ExternalPlayer();

	virtual bool init();
	virtual bool cleanup();
	virtual QString getName();

	virtual void receiveMsg(QString msg);

private slots:
	void onStdOut();
	void onStdErr();
	void onExit(int exitCode, QProcess::ExitStatus status);

private:
	void sendStop();
	void sendCont();

	QProcess process;
	QStringList arguments;
	QString cmd;
};

#endif
