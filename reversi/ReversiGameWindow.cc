#include "ReversiGameWindow.h"

#include "ReversiGameState.h"

#include <QDebug>

ReversiGameWindow::ReversiGameWindow(ReversiGameState *state)
	: player(state)
{
	ui.setupUi(this);
	ui.widget->setGameState(state);
	connect(ui.widget, SIGNAL(moveSelected(QString)), this, SLOT(onMoveSelected(QString)));
	connect(&player, SIGNAL(msgReceivedSignal(QString)), this, SLOT(onMsgReceived(QString)));
	updatePoints();
}

void ReversiGameWindow::updatePoints()
{
	int black = player.getGameState()->getPoints(ReversiGameState::BLACK_PLAYER);
	int white = player.getGameState()->getPoints(ReversiGameState::WHITE_PLAYER);
	int currentPlayer = player.getGameState()->getCurrentPlayer();

	ui.blackPoints->setText(QString::number(black));
	ui.whitePoints->setText(QString::number(white));

	QString blackText = (currentPlayer == ReversiGameState::BLACK_PLAYER) ? "<b>BLACK:</b>" : "BLACK:";
	QString whiteText = (currentPlayer == ReversiGameState::WHITE_PLAYER) ? "<b>WHITE:</b>" : "WHITE:";

	ui.blackLabel->setText(blackText);
	ui.whiteLabel->setText(whiteText);

}

void ReversiGameWindow::onMoveSelected(QString move)
{
	if (player.getGameState()->getPlayer() != player.getGameState()->getCurrentPlayer())
		return;

	if (!player.getGameState()->makeMove(move))
		return;

	update();
	player.sendMsg(move);
}

void ReversiGameWindow::onMsgReceived(QString msg)
{
	if (!player.getGameState()->makeMove(msg))
		return;
	update();
}

void ReversiGameWindow::on_passButton_clicked()
{
	onMoveSelected("pass");
}

void ReversiGameWindow::paintEvent(QPaintEvent *event)
{
	updatePoints();
	QMainWindow::paintEvent(event);
}
