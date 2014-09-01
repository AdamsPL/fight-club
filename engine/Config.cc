#include "Config.h"

#include <QSettings>
#include <QStringList>

void Config::loadFromFile(QString filename)
{
	QSettings iniFile(filename, QSettings::IniFormat);
	QString key;

	foreach(key, iniFile.allKeys()) {
		storedValues[key] = iniFile.value(key).toString();
	}
}

const QString Config::getValue(QString label) const
{
	if (!storedValues.contains(label))
		return QString();

	return storedValues.value(label);
}
