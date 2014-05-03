#include "ReversiRules.h"

#include "ReversiGameWindow.h"
#include "ReversiGameState.h"

#include <unistd.h>

ReversiRules::ReversiRules()
	: lastMove("* *"), state(NULL), /*ui(NULL), server(NULL),*/ checkTime(false)
{
	timeLeft[ReversiGameState::BLACK_PLAYER] = 0;
	timeLeft[ReversiGameState::WHITE_PLAYER] = 0;
}

QStringList ReversiRules::getExternalPlayerArgs(int player)
{
	QString playerString = (player == ReversiGameState::BLACK_PLAYER) ? "b" : "w";
	QString playerParam = getPlayerParamName(player);
	int boardSize = getBoardSize() / 2;
	QStringList result;

	if (boardSize < 0)
		return QStringList();
	
	result << QString::number(boardSize) << playerString;

	if (checkTime)
		result << QString::number(timeLeft[player]);

	return result;
}

GameState *ReversiRules::createGameState(int player)
{
	int boardSize = getBoardSize();

	if (boardSize < 0)
		return NULL;

	return new ReversiGameState(player, boardSize);
}

Engine::GameResult playerToGameResult(int player)
{
	if (player == 0)
		return Engine::PlayerZeroWon;
	else
		return Engine::PlayerOneWon;
}

Engine::MoveResult ReversiRules::returnMoveResult(Engine::MoveResult res, Engine::GameResult gameRes)
{
	setGameResult(gameRes);
	return res;
}

Engine::MoveResult ReversiRules::validateMove(int player, QString move, int elapsed)
{
	int p0 = getPoints(0);
	int p1 = getPoints(1);

	if (checkTime) {
		timeLeft[player] -= elapsed;
		if (timeLeft[player] < 0) {
			return returnMoveResult(Engine::Timeout, playerToGameResult(getNextPlayer(player)));
		}
	}

	if (move == "pass" && lastMove == "pass") {
		if (p0 > p1)
			return returnMoveResult(Engine::ValidMove, Engine::PlayerZeroWon);
		else if (p1 > p0)
			return returnMoveResult(Engine::ValidMove, Engine::PlayerOneWon);
		else
			return returnMoveResult(Engine::ValidMove, Engine::Tie);

		return Engine::ValidMove;
	}
	lastMove = move;

	if (state->makeMove(move)) {
		/*
		if (ui) {
			ui->update();
			usleep(500000);
		}
        broadcastState();
		*/
		return Engine::ValidMove;
	} else {
		return returnMoveResult(Engine::InvalidMove, playerToGameResult(getNextPlayer(player)));
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

bool ReversiRules::readParams(Engine *engine)
{
	const int inactivePlayer = 3;
	QString boardSizeText = engine->getParam("size");
	QString uiText = engine->getParam("enable-ui");
	QString timeoutText = engine->getParam("timeout");

	if (boardSizeText.isEmpty())
		return false;

	boardSize = boardSizeText.toInt();
	/*
	hasUI = (uiText.toLower() == "true");
	*/
		
	if (!timeoutText.isEmpty()) {
		int time = timeoutText.toInt();
		checkTime = true;
		timeLeft[ReversiGameState::BLACK_PLAYER] = time;
		timeLeft[ReversiGameState::WHITE_PLAYER] = time;
	}

	state = static_cast<ReversiGameState*>(createGameState(inactivePlayer));
	return true;
}

int ReversiRules::getPoints(int player)
{
	int result;

	result = state->getPoints(player);

	if (getGameResult() == Engine::PlayerZeroWon && player == 0)
		result += state->getEmpty();
	if (getGameResult() == Engine::PlayerOneWon && player == 1)
		result += state->getEmpty();

	return result;
}

/*
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
*/

void ReversiRules::onPlayerLeave(int player)
{
	if (getGameResult() != Engine::Undefined)
		return;

	setGameResult(playerToGameResult(getNextPlayer(player)));
}
