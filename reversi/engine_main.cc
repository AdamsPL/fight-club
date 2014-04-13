#include "ReversiRules.h"
#include <engine/Engine.h>

#include <QApplication>
#include <QDebug>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	ReversiRules rr;
	Engine engine(&rr);

	rr.parseArgs(app.arguments());

	Player *p0 = engine.loadExternalPlayer(0);
	Player *p1 = engine.loadExternalPlayer(1);

	engine.registerPlayer(p0);
	engine.registerPlayer(p1);
    
	return app.exec();
}
