#ifndef _LOGGER_H
#define _LOGGER_H

#include "PlayerListener.h"

#include <QVector>
#include <QStringList>
#include <QFile>
#include <QTextStream>

class Logger : public PlayerListener
{
public:
	Logger(QString filename);
	virtual ~Logger();

	void startRecord(QStringList gameArgs);
	QStringList startPlayback();
	QString getNextMove();
	virtual void receiveMsg(int fromPlayer, QString msg);

	static QString getPlayerParamSeparator() { return separator; }

private:
	QFile file;
	QTextStream stream;
	static QString separator;
};

#endif
