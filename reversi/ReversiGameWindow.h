#ifndef _REVERSI_GAME_WINDOW_H
#define _REVERSI_GAME_WINDOW_H

#include <QMainWindow>

#include "ui_ReversiWindow.h"
#include <engine/Player.h>

class ReversiGameState;

class ReversiGameWindow : public QMainWindow
{
	Q_OBJECT

public:
	ReversiGameWindow(ReversiGameState *state);
	inline Player *getPlayer() { return &player; }

signals:
	void msgReceivedSignal(QString msg);

protected:
	virtual void paintEvent(QPaintEvent *event);

private slots:
	void onMoveSelected(QString move);
	void onMsgReceived(QString msg);
	void on_passButton_clicked();

private:
	void receiveMsg(QString msg);
	Player player;
	void updatePoints();
	Ui::MainWindow ui;
};

#endif
