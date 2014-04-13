#include "ReversiGameState.h"
#include "ReversiGameWindow.h"
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
	ReversiGameState *uiState;
	ReversiGameState *stdoutState;

	if (rules.getPlayer() == rgsBlack.getPlayer()) {
		uiState = &rgsBlack;
		stdoutState = &rgsWhite;
	} else {
		uiState = &rgsWhite;
		stdoutState = &rgsBlack;
	}

	StdoutPlayer stdint(stdoutState);
	ReversiGameWindow window(uiState);

	engine.registerPlayer(window.getPlayer());
	engine.registerPlayer(&stdint);

	stdint.start();
	window.show();

	return app.exec();
}
