#include "GameState.h"

#include <cfloat>
#include <algorithm>

#include <QDebug>

GameState::GameState(int player)
	: player(player), currentPlayer(0)
{
}

/*
GameState::GameState(Player player, bool canMove)
	: player(player), parent(NULL), canMove(canMove), points(0.0f), refCount(1), disabled(false)
{
}

GameState::GameState(GameState *p)
	: parent(p), player(p->player), canMove(!p->canMove), points(p->points), refCount(1), disabled(false)
{
}

GameState *GameState::getChild(QString move)
{
	if (!children.contains(move))
		return NULL;
	else
		return children[move];
}

GameState::~GameState()
{
	GameState *child;
	foreach (child, children.values()) {
		child->parent = NULL;
		child->deref();
	}
}

void GameState::updatePoints()
{
	GameState *child;
	float best = (canMove) ? -FLT_MAX : FLT_MAX;
	bool changed;

	if (children.isEmpty()) {
		best = getHeuristic();
	} else {
		foreach(child, children.values()) {
			if (canMove)
				best = std::max(best, child->points);
			else
				best = std::min(best, child->points);
		}
	}
	changed = (points != best);
	points = best;
	if (parent && changed)
		parent->updatePoints();
}

void GameState::addChild(QString move, GameState *child)
{
	children[move] = child;
}

GameState *GameState::selectMove(Player player, QString move)
{
	GameState *child;

	if (player != getCurrentPlayer()) {
		return NULL;
	}
	if (!children.contains(move)) {
		return NULL;
	}

	child = children[move];
	child->parent = NULL;
	child->expand();

	children.remove(move);
	disableTree();

	return child;
}

GameState::Player GameState::getCurrentPlayer()
{
	if (canMove)
		return player;
	else
		return getOppositePlayer(player);
}

int GameState::expandNodes(int maxDepth)
{
	GameState *child;
	int result = 1;

	if (maxDepth <= 0)
		return 0;

	--maxDepth;

	expand();
	foreach (child, children.values())
		result += child->expandNodes(maxDepth);

	return result;
}

QString GameState::getBestMove()
{
	QString move;
	QString result("pass");
	float best = (canMove) ? -FLT_MAX : FLT_MAX;

	qDebug() << "Finding BEST MOVE...";

	foreach(move, children.keys()) {
		float cur = children[move]->points;
		qDebug() << "  checking " << move << cur << " [best:" << best << "]";
		if (!canMove && cur < best) {
			qDebug() << "WORSE!";
			best = cur;
			result = move;
		}
		if (canMove && cur > best) {
			qDebug() << "BETTER!";
			best = cur;
			result = move;
		}
	}

	return result;
}

void GameState::ref()
{
	refCount.ref();
}

void GameState::deref()
{
	if (!refCount.deref())
		delete this;
}

void GameState::disableTree()
{
	GameState *child;
	disabled = true;
	
	foreach (child, children.values())
		child->disableTree();
}
*/
