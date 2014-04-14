#include "ReversiRules.h"

#include "ReversiGameWindow.h"
#include "ReversiGameState.h"

#include <unistd.h>
#include <QDebug>

ReversiRules::ReversiRules()
	: lastMove("* *"), gameResult(Engine::Undefined), state(NULL), ui(NULL), server(NULL)
{
}

QStringList ReversiRules::getExternalPlayerArgs(int player)
{
	QString playerString = (player == ReversiGameState::BLACK_PLAYER) ? "b" : "w";
	QString playerParam = getPlayerParamName(player);
	int boardSize = getBoardSize() / 2;

	if (boardSize < 0)
		return QStringList();

	return (getParamList(playerParam) << QString::number(boardSize) << playerString);
}

GameState *ReversiRules::createGameState(int player)
{
	int boardSize = getBoardSize();

	if (boardSize < 0)
		return NULL;

	return new ReversiGameState(player, boardSize);
}

bool ReversiRules::validateMove(int player, QString move)
{
	int p0 = getPoints(0);
	int p1 = getPoints(1);

	if (move == "pass" && lastMove == "pass") {
		if (p0 > p1)
			gameResult = Engine::PlayerZeroWon;
		else if (p1 > p0)
			gameResult = Engine::PlayerOneWon;
		else
			gameResult = Engine::Tie;

		return true;
	}
	lastMove = move;

	if (state->makeMove(move)) {
		if (ui) {
			ui->update();
			usleep(200000);
		}
        broadcastState();
		return true;
	} else {
		if (player == 0)
			gameResult = Engine::PlayerOneWon;
		else
			gameResult = Engine::PlayerZeroWon;
		return false;
	}
}

bool ReversiRules::parsePlayerArgs(QStringList args)
{
	if (args.size() < 2)
		return false;

	boardSize = args[0].toInt();
	if (args[1].startsWith('b', Qt::CaseInsensitive))
		player = ReversiGameState::BLACK_PLAYER;
	else
		player = ReversiGameState::WHITE_PLAYER;

	return true;
}

bool ReversiRules::checkParams()
{
	QString boardSizeText = getParam("size");
	QString uiText = getParam("enable-ui");

	if (boardSizeText.isEmpty())
		return false;

	boardSize = boardSizeText.toInt();
	hasUI = (uiText.toLower() == "true");
		
	return true;
}

bool ReversiRules::init()
{
	static const int inactivePlayer = 3;
	state = static_cast<ReversiGameState*>(createGameState(inactivePlayer));
	if (hasUI) {
		ui = new ReversiGameWindow(state);
		ui->show();
	}

    server = new WebsocketServer();
}

int ReversiRules::getPoints(int player)
{
	int result;

	if (!state)
		return -1;

	result = state->getPoints(player);

	if (gameResult == Engine::PlayerZeroWon && player == 0)
		result += state->getEmpty();
	if (gameResult == Engine::PlayerOneWon && player == 1)
		result += state->getEmpty();

	return result;
}

void ReversiRules::broadcastState()
{
    QString sizeStr = QString::number(getBoardSize() / 2);
    QString boardStr;

    for (int row = 0; row < getBoardSize(); ++row) {
        for (int col = 0; col < getBoardSize(); ++col) {
            switch(state->getStatus(col, row)) {
                case ReversiGameState::BLACK_PLAYER: boardStr.append('b'); break;
                case ReversiGameState::WHITE_PLAYER: boardStr.append('w'); break;
                case ReversiGameState::EMPTY: boardStr.append('.'); break;
                default: break;
            }
        }
    }
    QString p0Time = "5000";
    QString p1Time = "5000";

    QString p0Name = "player0";
    QString p1Name = "player1";

    QString separator = "|";

    QStringList params;
    params << sizeStr << boardStr << lastMove << p0Time << p1Time << p0Name << p1Name;
    
    QString msg = params.join(separator);

    server->broadcast(msg);
}

Engine::GameResult ReversiRules::getGameResult()
{
	return gameResult;
}

void ReversiRules::onPlayerLeave(int player)
{
	if (gameResult != Engine::Undefined)
		return;

	if (player == 0) 
		gameResult = Engine::PlayerOneWon;
	else
		gameResult = Engine::PlayerZeroWon;
}
