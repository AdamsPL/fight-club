#include "ReversiRules.h"
#include <engine/Engine.h>

#include <QApplication>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	ReversiRules rr;
	Engine engine(&rr);

	if (!rr.parseArgs(app.arguments()))
		return -1;

	Player *p0 = engine.loadExternalPlayer(0);
	Player *p1 = engine.loadExternalPlayer(1);

	engine.registerPlayer(p0);
	engine.registerPlayer(p1);
    
	return app.exec();
}
