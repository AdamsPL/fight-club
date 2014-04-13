#include "ReversiRules.h"

#include "ReversiGameWindow.h"
#include "ReversiGameState.h"

#include <unistd.h>
#include <QDebug>

ReversiRules::ReversiRules()
	: lastMove("* *"), winner(-1), state(NULL), server(NULL)
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

Engine::MoveResult ReversiRules::validateMove(int player, QString move)
{
	if (move.toLower() == "pass" && lastMove.toLower() == "pass") {
		if (getPoints(player) > getPoints(getNextPlayer(player)))
			winner = player;
		else
			winner = getNextPlayer(player);
		return Engine::FinishingMove;
	}
	lastMove = move;

	usleep(100000);

	if (state->makeMove(move)) {
		ui->update();
        broadcastState();
		return Engine::ValidMove;
	} else
		return Engine::InvalidMove;
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

	if (boardSizeText.isEmpty())
		return false;

	boardSize = boardSizeText.toInt();
	return true;
}

bool ReversiRules::init()
{
	static const int inactivePlayer = 3;
	state = static_cast<ReversiGameState*>(createGameState(inactivePlayer));
	ui = new ReversiGameWindow(state);
	ui->show();

    server = new WebsocketServer();
}

int ReversiRules::getWinner()
{
	return winner;
}

int ReversiRules::getPoints(int player)
{
	int result;

	if (!state)
		return -1;

	result = state->getPoints(player);

	if (winner == player)
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
