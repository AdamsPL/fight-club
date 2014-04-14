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
	cmd = arguments.first();
	arguments.pop_front();
    process.start(cmd, arguments, QIODevice::ReadWrite | QIODevice::Unbuffered);
    return process.waitForStarted(-1);
}

bool ExternalPlayer::cleanup()
{
    process.close();
	return true;
}

void ExternalPlayer::onStdOut()
{
    while(process.canReadLine()) {
        sendMsg(process.readLine().trimmed());
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
