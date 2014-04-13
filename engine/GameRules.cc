#include "GameRules.h"

bool GameRules::parseArgs(QStringList args)
{
	QString lastParam;
	QString tmp;

	foreach (tmp, args) {
		if (tmp.startsWith("--")) {
			lastParam = tmp.remove("--");
			parameters[lastParam] = QStringList();
			continue;
		}
		if (lastParam.isEmpty())
			continue;

		parameters[lastParam] << tmp;
	}
	if (!checkParams())
		return false;
	init();
	return true;
}

QString GameRules::getParam(QString name)
{
	if (!parameters.contains(name))
		return QString();
	if (parameters[name].isEmpty())
		return QString();
	return parameters[name].first();
}

QStringList GameRules::getParamList(QString name)
{
	if (!parameters.contains(name))
		return QStringList();
	return parameters[name];
}

QString GameRules::getPlayerParamName(int player)
{
	static const QString prefix = "player";
	return prefix + QString::number(player);
}
