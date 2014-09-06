#ifndef _CONFIG_H
#define _CONFIG_H 

#include <QString>
#include <QMap>

class Config
{
public:
	void loadFromFile(QString filename);
	const QString getValue(QString label) const;

private:
	QMap<QString, QString> storedValues;
};

#endif /* _CONFIG_H */
