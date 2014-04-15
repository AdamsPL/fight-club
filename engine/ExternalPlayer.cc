#include "ExternalPlayer.h"

#include <iostream>

#include <QFileInfo>

#ifdef __LINUX__
#include <sys/types.h>
#include <signal.h>
#endif

using namespace std;

ExternalPlayer::ExternalPlayer(GameState *state, QStringList arguments)
    : Player(state), arguments(arguments)
{
    process.setReadChannel(QProcess::StandardOutput);
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(onStdOut()));
    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(onStdErr()));
    connect(&process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onExit(int, QProcess::ExitStatus)));
}

ExternalPlayer::~ExternalPlayer()
{
}

bool ExternalPlayer::init()
{
	bool started;

	cmd = arguments.first();
	arguments.pop_front();
    process.start(cmd, arguments, QIODevice::ReadWrite | QIODevice::Unbuffered);
	started = process.waitForStarted(-1);
	lastTimestamp.start();

	return started;
}

bool ExternalPlayer::cleanup()
{
    process.close();
	return true;
}

void ExternalPlayer::onStdOut()
{
	int elapsed = lastTimestamp.elapsed();
    while(process.canReadLine()) {
        sendMsg(process.readLine().trimmed(), elapsed);
    }
	sendStop();
}

void ExternalPlayer::onStdErr()
{
	cout << process.readAllStandardError().trimmed().data() << endl;
}

void ExternalPlayer::onExit(int exitCode, QProcess::ExitStatus status)
{
	leave();
}

void ExternalPlayer::receiveMsg(QString msg)
{
    process.write(msg.append("\n").toAscii());
	lastTimestamp.start();
	sendCont();
}

void ExternalPlayer::sendStop()
{
#ifdef __LINUX__
	kill(process.pid(), SIGSTOP);
#endif
}

void ExternalPlayer::sendCont()
{
#ifdef __LINUX__
	kill(process.pid(), SIGCONT);
#endif
}

QString ExternalPlayer::getName()
{
	QFileInfo info(cmd);
	return info.baseName();
}
