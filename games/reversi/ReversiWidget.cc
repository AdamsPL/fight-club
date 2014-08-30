#include "ReversiWidget.h"

#include "ReversiGameState.h"

#include <algorithm>

#include <QPainter>
#include <QMouseEvent>

#include "ui_ReversiWindow.h"

ReversiWidget::ReversiWidget(QWidget *parent)
	: QWidget(parent), state(NULL), boardWidth(1)
{
}

void ReversiWidget::recalcSize()
{
	boardWidth = std::min(width(), height()) - margin * 2;
	leftMargin = (width() - boardWidth) / 2;
	topMargin = (height() - boardWidth) / 2;
}

void ReversiWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	recalcSize();

	painter.setRenderHint(QPainter::Antialiasing);

	drawOutline(painter);
	drawBoard(painter);
	drawPossible(painter);
}

void ReversiWidget::drawOutline(QPainter &painter)
{
	painter.setBrush(QBrush(Qt::darkGreen));
	painter.fillRect(0, 0, width(), height(), Qt::SolidPattern);

	painter.setBrush(QBrush(Qt::darkGray));
	painter.drawRect(leftMargin, topMargin, boardWidth, boardWidth);
}

void ReversiWidget::drawBoard(QPainter &painter)
{
	if (!state)
		return;

	int boardColumns = state->getBoardSize();
	float cellWidth = (float)boardWidth / boardColumns;

	painter.setBrush(QBrush(Qt::darkGray));
	for (int r = 1; r < boardColumns; ++r) {
		painter.drawLine(
			leftMargin + (int)(r * cellWidth),
			topMargin,
			leftMargin + (int)(r * cellWidth),
			topMargin + boardWidth
		);
		painter.drawLine(
			leftMargin,
			topMargin + (int)(r * cellWidth),
			leftMargin + boardWidth,
			topMargin + (int)(r * cellWidth)
		);

	}

	for (int r = -1; r <= boardColumns; ++r) {
		for (int c = -1; c <= boardColumns; ++c) {
			drawCell(
				painter,
				(int)(leftMargin + c * cellWidth),
				(int)(topMargin + r * cellWidth),
				(int)(cellWidth),
				state->getStatus(c, r)
			);
		}
	}
}

void ReversiWidget::drawPossible(QPainter &painter)
{
	if (!state)
		return;

	int boardColumns = state->getBoardSize();
	float cellWidth = (float)boardWidth / boardColumns;
	float phi = 2;

	for (int r = 0; r < boardColumns; ++r) {
		for (int c = 0; c < boardColumns; ++c) {
			int tmp = state->flip(c, r, true);
			if (tmp <= 0)
				continue;
			int posX = leftMargin + cellWidth * c + cellWidth / 2;
			int posY = topMargin + cellWidth * r + cellWidth / 2;
			painter.setBrush(QBrush(Qt::blue));
			painter.drawEllipse(posX - phi, posY - phi, phi * 2, phi * 2);
		}
	}
}

void ReversiWidget::drawCell(QPainter &painter, int x, int y, int width, int state)
{
	static const int cellMargin = 4;
	x+= cellMargin;
	y+= cellMargin;
	width -= cellMargin * 2;

	switch(state)
	{
		case ReversiGameState::WHITE_PLAYER:{
			painter.setBrush(QBrush(Qt::white));
			painter.drawEllipse(x, y, width, width);
			break;
		}
		case ReversiGameState::BLACK_PLAYER:{
			painter.setBrush(QBrush(Qt::black));
			painter.drawEllipse(x, y, width, width);
			break;
		}

		default: break;
	}
}

void ReversiWidget::mousePressEvent(QMouseEvent *event)
{
	int boardColumns = state->getBoardSize();
	float cellWidth = (float)boardWidth / boardColumns;
	int x = (int)((event->x() - leftMargin) / cellWidth);
	int y = (int)((event->y() - topMargin) / cellWidth);

	QString msg = QString("%1 %2").arg(x).arg(y);

	emit moveSelected(msg);
}
