#include <ExternalProgram.h>

ExternalProgram::ExternalProgram(QString *configFile)
{
	config.load(configFile);
}

ExternalPorgram::~ExternalProgram()
{
}
