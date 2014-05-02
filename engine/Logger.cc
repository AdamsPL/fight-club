#include "Logger.h"

QString Logger::separator = "|";

Logger::Logger(QString filename)
	: file(filename), stream(&file)
{
	file.open(QFile::ReadWrite);
}

Logger::~Logger()
{
	file.close();
}

void Logger::startRecord(QStringList gameArgs)
{
	stream << gameArgs.join(separator) << endl;
}

void Logger::receiveMsg(int fromPlayer, QString msg)
{
	stream << msg << endl;
}

QStringList Logger::startPlayback()
{
	QString params = stream.readLine();
	return params.split(separator, QString::SkipEmptyParts);
}

QString Logger::getNextMove()
{
	return stream.readLine();
}
