#ifndef _REVERSI_WIDGET_H
#define _REVERSI_WIDGET_H

#include <QWidget>

class QPainter;
class ReversiGameState;

class ReversiWidget : public QWidget
{
	Q_OBJECT

public:
	ReversiWidget(QWidget *parent);
	inline void setGameState(ReversiGameState *state) { this->state = state; }

signals:
	void moveSelected(QString move);

protected:
	virtual void paintEvent(QPaintEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);

private:
	void recalcSize();
	void drawBoard(QPainter &painter);
	void drawOutline(QPainter &painter);
	void drawPossible(QPainter &painter);
	void drawCell(QPainter &painter, int x, int y, int width, int state);

	ReversiGameState *state;
	int boardWidth;
	int leftMargin;
	int topMargin;
	static const int margin = 10;
};

#endif
