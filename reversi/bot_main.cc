#include "SimpleAI.h"
#include "ReversiRules.h"

#include <engine/StdoutPlayer.h>
#include <engine/Engine.h>

#include <QApplication>

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	ReversiRules rules;
	Engine engine;

	QStringList args = app.arguments();
	args.pop_front();

	rules.parsePlayerArgs(args);

	ReversiGameState rgsBlack(ReversiGameState::BLACK_PLAYER, rules.getBoardSize());
	ReversiGameState rgsWhite(ReversiGameState::WHITE_PLAYER, rules.getBoardSize());
	ReversiGameState *aiState;
	ReversiGameState *stdoutState;

	if (rules.getPlayer() == rgsBlack.getPlayer()) {
		aiState = &rgsBlack;
		stdoutState = &rgsWhite;
	} else {
		aiState = &rgsWhite;
		stdoutState = &rgsBlack;
	}

	StdoutPlayer stdint(stdoutState);
	SimpleAI ai(aiState);

	engine.registerPlayer(&ai);
	engine.registerPlayer(&stdint);

	stdint.start();

	return app.exec();
}
