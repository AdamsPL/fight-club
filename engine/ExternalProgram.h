#ifndef _EXTERNALPROGRAM_H
#define _EXTERNALPROGRAM_H 

#include <QString>

class ExternalProgram
{
public:
	ExternalProgram(QString configFile);

private:
	Config config;
};

#endif /* _EXTERNALPROGRAM_H */
